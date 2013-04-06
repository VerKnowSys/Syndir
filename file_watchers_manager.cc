/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "syndir.h"

#ifdef GUI_ENABLED
    #include "screenshot_sync_app/synshot.h"
#endif


FileWatchersManager::~FileWatchersManager() {
    qDebug() << "Shutting down";
    // libssh2_sftp_shutdown(sftp_session);
    delete connection;
}


FileWatchersManager::FileWatchersManager(const QString& sourceDir, const QString& fullDestinationSSHPath, bool convertToSha) {
    qDebug() << "Starting recursive watch on dir:" << sourceDir << "with sync to remote:" << fullDestinationSSHPath;
    this->performNameConvertionToShaAndCopyToClipboard = convertToSha; /* used only for preprocessing of files sent to remote - f.e. sending screenshots */
    this->baseCWD = sourceDir;
    this->fullDestinationSSHPath = fullDestinationSSHPath; /* f.e: someuser@remotehost:/remote/path */

    /* user might be implicit: */
    QStringList sshDirPartial = fullDestinationSSHPath.split(":");
    QString userWithHost = sshDirPartial.value(0);
    QStringList sshUserServerPartial = userWithHost.split("@"); /* first part is user@host */
    QRegExp emailSign("@");
    if (not userWithHost.contains(emailSign)) {
        this->userName = getenv("USER"); /* set username if not given explicitly */
        this->hostName = sshUserServerPartial.value(0);
        // qDebug() << "Implicit" << userName << hostName;
    } else { /* or explicit */
        // qDebug() << "Explicit";
        this->userName = sshUserServerPartial.value(0);
        this->hostName = sshUserServerPartial.value(1);
    }
    this->remotePath = sshDirPartial.value(1); /* last one is a remote path */

    if (this->userName.isEmpty()) {
        qDebug() << "No user name specified, trying to get it from ENV.";
        this->userName = getenv("USER");
    }

    if (this->hostName.isEmpty() or this->userName.isEmpty()) {
        qDebug() << "Invalid (empty) host or user name.";
        usage();
        exit(1);
    }

    /* connect hooks to invokers */
    connect(this, SIGNAL(fileChanged(QString)), this, SLOT(fileChangedSlot(QString)));
    connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(dirChangedSlot(QString)));

    scanDir(QDir(sourceDir)); /* will fill up manager 'files' field */
}


void FileWatchersManager::connectToRemoteHost() {
    while ((connection == NULL) or (not connection->isSessionValid())) {
        try {
            QSettings settings;
            if (connection != NULL)
                delete connection;
            connection = new Connection(hostName.toStdString(), settings.value("ssh_port", SSH_PORT).toInt(), true);
            connection->setKeyPath(keysLocation.toStdString());
            connection->setCredentials(userName.toStdString(), "");
            connection->mkConnection();
            qDebug() << "Connected as:" << userName + "@" + hostName;

        } catch (Exception& e) {
            qDebug() << "Error connecting to remote host:" << e.what() << "\nWill retry!";
            sleep(1);
            return connectToRemoteHost();
        }
    }
}


/* by tallica & dmilith */
void FileWatchersManager::scanDir(QDir dir) {
    qDebug() << "Scanning:" << dir.absolutePath();
    // disconnect(SIGNAL(fileChanged(QString)));
    // disconnect(SIGNAL(directoryChanged(QString)));


    this->oldFiles = this->files;
    for (int index = 0; index < files.size(); index++) {
        removePath(files.at(index));
        files.removeAt(index);
    }

    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QDirIterator it(dir, QDirIterator::Subdirectories);
    files << dir.absolutePath(); /* required for dir watches */

    while (it.hasNext()) {
        QString nextOne = it.next();
        if (QDir(nextOne).exists()) {
            // qDebug() << "DIRECTORY:" << nextOne;
            files << nextOne;
        } else {
            QRegExp matcher(settings.value("allowed_file_types", ALLOWED_FILE_TYPES).toString());
            if (nextOne.contains(matcher)) {
                // qDebug() << "Found match:" << nextOne;
                files << nextOne;
            }
        }
    }
    files.removeDuplicates();

    addPaths(files);
    qDebug() << "Total files and dirs on watch:" << files.size();
    qDebug() << "Ready.";
}


void FileWatchersManager::fileChangedSlot(const QString& file) {
    /* compare file name and modification date. Ignore doubles */
    if ((last != file) || (QFileInfo(file).created() != this->lastModified)) {
        this->lastModified = QFileInfo(file).created();
        this->last = file;
        /* use case for auto uploading screenshots to remote site with auto hash generation and copy to clipboard */
        if (this->performNameConvertionToShaAndCopyToClipboard)
            copyFileToRemoteHost(file, true);
        else
            copyFileToRemoteHost(file);
    } //else
        // qDebug() << "Sync not necessary for:" << file;
}


void FileWatchersManager::copyFileToRemoteHost(const QString& sourceFile, bool hashFile) {
    QString file = sourceFile;
    #ifdef GUI_ENABLED
        auto fileInfo = QFileInfo(file);
        auto dir = fileInfo.absolutePath();
        auto extension = fileInfo.suffix();

        auto hash = new QCryptographicHash(QCryptographicHash::Sha1);
        hash->addData(file.toUtf8(), file.length() + 1);
        auto result = hash->result().toHex();
        delete hash;

        auto renamedFile = dir + "/" + result + "." + extension;
        auto clipboard = QApplication::clipboard();
        clipboard->setText(settings.value("remote_path", REMOTE_PATH).toString() + result + "." + extension);
    #endif

    QString fileDirName = QFileInfo(file).absolutePath();
    QStringRef prePath(&file, baseCWD.size(), (file.size() - baseCWD.size()));
    QStringRef preDirs(&fileDirName, baseCWD.size(), (fileDirName.size() - baseCWD.size()));
    QString chopFileName = prePath.toUtf8();
    QString fullDestPath = remotePath + chopFileName;

    connectToRemoteHost();

    /* create session here */
    LIBSSH2_SFTP *sftp_session = NULL;
    while (sftp_session == NULL) {
        /* connect if not connected */
        sftp_session = libssh2_sftp_init(connection->session);
        cout << ".";
        fflush(stdout);
        usleep(100);
    }

    libssh2_session_set_timeout(connection->session, DEFAULT_SESSION_TIMEOUT);
    libssh2_session_set_blocking(connection->session, 1); /* set session to blocking */

    if (not QFile::exists(file)) {
        qDebug() << "Deletion detected:" << file;
        qDebug() << "Synced deletion of remote file:" << fullDestPath;
        libssh2_sftp_unlink(sftp_session, fullDestPath.toUtf8());
        removePath(file);
        return;
    }

    /* creating dir recursively for a single destination file */
    auto finalPath = remotePath.toUtf8();

    /* Read permissions of a source file */
    libssh2_sftp_mkdir(sftp_session, finalPath, 0755);
    if (not preDirs.isEmpty()) { /* sub dirs in path */
        auto elems = preDirs.toString().split("/");
        for (int i = 0; i < elems.length(); i++) {
            auto elem = elems.at(i);
            finalPath += "/" + elem;
            if (not elem.isEmpty()) {
                libssh2_sftp_mkdir(sftp_session, finalPath, 0755);
            }
        }
    }

    qDebug() << endl << "Detected modification of:" << file << "Syncing to:" << hostName;

    /* Read permissions of a source file */
    struct stat results;
    stat(file.toUtf8(), &results);

    /* Request a file via SFTP */
    LIBSSH2_SFTP_HANDLE *sftp_handle = NULL;
    while (sftp_handle == NULL) { /* it's case when network connection is overloaded */
        sftp_handle = libssh2_sftp_open(sftp_session, fullDestPath.toUtf8(), LIBSSH2_FXF_READ|LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC, results.st_mode);
        cout << ".";
        fflush(stdout);
        usleep(100);
    }
    ifstream fin(file.toUtf8(), ios::binary);
    if (fin) {
        fin.seekg(0, ios::end);
        long bufsize = fin.tellg(); /* get file size in bytes */

        long BUFF = bufsize;
        if (bufsize > MAXBUF) /* set buffer to 12 KiB if file size exceeds limit */
            BUFF = MAXBUF;

        qDebug() << "File size:" << bufsize/1024 << "KiB. Memory buffer size:" << BUFF << "bytes";
        fin.seekg(0); /* rewind to beginning of file */
        qDebug() << "Streaming:" << file;

        char* buf = new char[BUFF];
        uint chunk = 0; /* used only to count % progress */
        while (fin.good()) {
            fin.read(buf, BUFF); /* read file contents into buffer; todo: sanity checks */
            int result = libssh2_sftp_write(sftp_handle, buf, fin.gcount()); /* write to remote file */
            switch (result) {
                case LIBSSH2_ERROR_ALLOC:
                    qDebug() << "Error allocating buffer with size:" << bufsize;
                    break;
            }
            chunk += 1;
            if (bufsize > 0) {
                int percent = (chunk * BUFF) * 100 / bufsize;
                cout << "(" << percent << "%) " << (chunk * BUFF)/1024 << '/' << bufsize/1024 << " KiB" << '\r';
                fflush(stdout);
            }
        }
        delete[] buf;
        if (bufsize < 1024)
            qDebug() << "\r(100%)" << bufsize << "Bytes sent.";
        else
            qDebug() << "\r(100%)" << bufsize/1024 << "KiB sent.";

        /* rename remote file to hash name */
        if (hashFile) {
            #ifdef GUI_ENABLED
                qDebug() << "Remote rename from" << fullDestPath << "to" << remotePath + "/" + result + "." + extension;
                libssh2_sftp_rename(sftp_session, fullDestPath.toUtf8(), (remotePath + "/" + result + "." + extension).toUtf8());
            #endif
        }
    }
    fin.close();
    libssh2_sftp_close(sftp_handle);
    libssh2_sftp_shutdown(sftp_session);
    #ifdef GUI_ENABLED
        QSettings settings;
        QSound::play(settings.value("sound_file", DEFAULT_SOUND_FILE).toString());
    #endif
    qDebug() << "Done.";
}


void FileWatchersManager::dirChangedSlot(const QString& dir) {
    qDebug() << "Dir changed:" << dir;
    if (not QDir(dir).exists()) {
        qDebug() << "Dir has gone. Assuming directory deletion of:" << dir;
        removePath(dir);
        scanDir(QDir(dir + "/.."));
    } else {
        scanDir(QDir(dir)); /* don't scan non existent directories */

        /* scan for new files by file list diff */
        Q_FOREACH(QString nextOne, files) {
            if (QFile(nextOne).exists()) {
                if (not oldFiles.contains(nextOne)) {
                    qDebug() << "New file found in monitored dir:" << nextOne;
                    fileChangedSlot(nextOne);
                }
            }
        }
    }
}
