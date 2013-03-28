/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "syndir.h"


FileWatchersManager::~FileWatchersManager() {
    qDebug() << "Shutting down";
    libssh2_sftp_shutdown(sftp_session);
    delete connection;
}


FileWatchersManager::FileWatchersManager(const QString& sourceDir, const QString& fullDestinationSSHPath) {
    this->baseCWD = sourceDir;
    this->fullDestinationSSHPath = fullDestinationSSHPath; /* f.e: someuser@remotehost:/remote/path */

    /* user might be implicit: */
    QStringList sshDirPartial = fullDestinationSSHPath.split(":");
    QString userWithHost = sshDirPartial.value(0);
    QStringList sshUserServerPartial = userWithHost.split("@"); /* first part is user@host */
    QRegExp emailSign("@");
    if (!userWithHost.contains(emailSign)) {
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
    }

    if (this->hostName.isEmpty() or this->userName.isEmpty()) {
        qDebug() << "Invalid (empty) host or user name.";
        usage();
        exit(1);
    }

    this->connection = new Connection(hostName.toStdString(), SSH_PORT, true);
    try {
        connection->setKeyPath(keysLocation.toStdString());
        connection->mkConnection();
        if (connection->isSessionValid()) {
            qDebug() << "Connection OK";
            qDebug() << "Connected as" << userName + "@" + hostName;
        } else {
            qDebug() << "SSH Connection failed! Check your public key configuration or look for typo in command line";
            exit(1);
        }

        /* create a session */
        sftp_session = libssh2_sftp_init(connection->session);
        if (!sftp_session) {
            qDebug() << "SFTP session failed!";
            return;
        }

    } catch (Exception & e) {
        qDebug() << "Exception caught: " << e.what();
        exit(1);
    }

    qDebug() << "Traversing paths";
    scanDir(QDir(sourceDir)); /* will fill up manager 'files' field */
    qDebug() << "Files list loaded.\nTotal files and dirs on watch:" << files.size();
}


/* by tallica & dmilith */
void FileWatchersManager::scanDir(QDir dir) {
    disconnect(SIGNAL(fileChanged(QString)));
    disconnect(SIGNAL(directoryChanged(QString)));
    removePaths(files);

    files.clear();
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks); // QDir::Dirs | QDir::Hidden |
    QDirIterator it(dir, QDirIterator::Subdirectories);
    files << dir.absolutePath(); /* required for dir watches */

    while (it.hasNext()) {
        QString nextOne = it.next();
        if (QDir(nextOne).exists()) {
            // qDebug() << "DIRECTORY:" << nextOne;
            files << nextOne;
        } else {
            QRegExp matcher(ALLOWED_FILE_TYPES);
            if (nextOne.contains(matcher)) {
                // qDebug() << "Found match:" << nextOne;
                files << nextOne;
            }
        }
    }

    /* connect hooks to invokers */
    addPaths(files);
    connect(this, SIGNAL(fileChanged(QString)), this, SLOT(fileChangedSlot(QString)));
    connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(dirChangedSlot(QString)));
}


void FileWatchersManager::fileChangedSlot(const QString& file) {
    copyFileToRemoteHost(file);
}


void FileWatchersManager::copyFileToRemoteHost(const QString& file) {
    if (not QFile::exists(file)) {
        qDebug() << "Deletion detected:" << file;
        removePath(file);
        return;
    }
    QString fileDirName = QFileInfo(file).absolutePath();
    QStringRef prePath(&file, baseCWD.size(), (file.size() - baseCWD.size()));
    QStringRef preDirs(&fileDirName, baseCWD.size(), (fileDirName.size() - baseCWD.size()));
    QString chopFileName = prePath.toUtf8();
    QString fullDestPath = remotePath + chopFileName;

    /* creating dir recursively for a single destination file */
    auto finalPath = remotePath.toUtf8();

    /* Read permissions of a source file */
    libssh2_sftp_mkdir(sftp_session, finalPath, 0755);
    if (!preDirs.isEmpty()) { /* sub dirs in path */
        auto elems = preDirs.toString().split("/");
        for (int i = 0; i < elems.length(); i++) {
            auto elem = elems.at(i);
            finalPath += "/" + elem;
            if (!elem.isEmpty()) {
                libssh2_sftp_mkdir(sftp_session, finalPath, 0755);
            }
        }
    }

    qDebug() << "Source file changed:" << file;
    qDebug() << "Destination path   :" << fullDestPath;

    /* Read permissions of a source file */
    struct stat results;
    stat(file.toUtf8(), &results);

    /* Request a file via SFTP */
    sftp_handle = libssh2_sftp_open(sftp_session, file.toUtf8(), LIBSSH2_FXF_READ, 0644);
    sftp_handle_dest = libssh2_sftp_open(sftp_session, fullDestPath.toUtf8(), LIBSSH2_FXF_READ|LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC, results.st_mode);
    if (!sftp_handle) {
        qDebug() << "Failed SFTP handle!";
        return;
    }
    ifstream fin(file.toUtf8(), ios::binary);
    if (fin) {
        fin.seekg(0, ios::end);
        ios::pos_type bufsize = fin.tellg(); /* get file size in bytes */
        fin.seekg(0); /* rewind to beginning of file */

        char* buf = new char[bufsize];
        fin.read(buf, bufsize); /* read file contents into buffer */
        libssh2_sftp_write(sftp_handle_dest, buf, bufsize); /* write to remote file */
        buf = NULL;
    }
    fin.close();

    libssh2_sftp_close(sftp_handle_dest);
    libssh2_sftp_close(sftp_handle);
    qDebug() << "SFTP Transfer Successful.";
}


void FileWatchersManager::dirChangedSlot(const QString& dir) {
    qDebug() << "Dir changed:" << dir;
    if (not QDir(dir).exists()) {
        qDebug() << "Dir has gone. Assuming directory deletion of:" << dir;
        removePath(dir);
        scanDir(QDir(dir + "/.."));
    } else
        scanDir(QDir(dir)); /* don't scan non existent directories */
}
