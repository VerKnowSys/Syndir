/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "syndir.h"


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
        qDebug() << "Implicit" << userName << hostName;
    } else { /* or explicit */
        qDebug() << "Explicit";
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

    qDebug() << "Traversing paths";
    scanDir(QDir(sourceDir)); /* will fill up manager 'files' field */
    qDebug() << "Total files:" << files.size();

    // for (int i = 0; i < files.size(); ++i) {
    //     auto entry = files.at(i);
    //     qDebug() << "Entry:" << entry;
        // new FileWatcher(entry, this);
    // }
}


/* by tallica & dmilith */
void FileWatchersManager::scanDir(QDir dir) {
    disconnect(SIGNAL(fileChanged(QString)));
    disconnect(SIGNAL(directoryChanged(QString)));
    removePaths(files);

    files.clear();
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks); // QDir::Dirs | QDir::Hidden |
    QDirIterator it(dir, QDirIterator::Subdirectories);
    files << dir.absolutePath();

    while (it.hasNext())
        files << it.next();

    /* connect hooks to invokers */
    addPaths(files);
    connect(this, SIGNAL(fileChanged(QString)), this, SLOT(fileChangedSlot(QString)));
    connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(dirChangedSlot(QString)));
}


void FileWatchersManager::fileChangedSlot(const QString& file) {
    copyFileToRemoteHost(file);
}


void FileWatchersManager::copyFileToRemoteHost(const QString& file) {

    try {
        Connection connection(hostName.toStdString(), SSH_PORT, true);
        QString keysLocation = QString(getenv("HOME")) + "/.ssh";
        connection.setKeyPath(keysLocation.toStdString());
        connection.mkConnection();

        LIBSSH2_SFTP *sftp_session = NULL;
        LIBSSH2_SFTP_HANDLE *sftp_handle = NULL, *sftp_handle_dest = NULL;

        qDebug() << "Trying to login to" << hostName << "as user" << userName;
        qDebug() << "Looking for SSH keys in:" << keysLocation;

        if (connection.isSessionValid())
            qDebug() << "Connection OK";

        /* create a session */
        sftp_session = libssh2_sftp_init(connection.session);
        if (!sftp_session) {
            qDebug() << "SFTP session failed!";
            return;
        }
        qDebug() << "SFTP session initialized";

        // qDebug() << "FILE:" << file;
        QString fileDirName = QFileInfo(file).absolutePath();
        // qDebug() << "BASE CWD:" << baseCWD;
        QStringRef prePath(&file, baseCWD.size(), (file.size() - baseCWD.size()));
        QStringRef preDirs(&fileDirName, baseCWD.size(), (fileDirName.size() - baseCWD.size()));
        // qDebug() << "PREPATH:" << prePath;
        // qDebug() << "PREDIRS:" << preDirs;
        QString chopFileName = prePath.toUtf8();
        QString fullDestPath = remotePath + chopFileName;

        /* creating dir recursively for a single destination file */
        auto finalPath = remotePath.toUtf8();
        libssh2_sftp_mkdir(sftp_session, finalPath, 0775);
        if (!preDirs.isEmpty()) { /* sub dirs in path */
            auto elems = preDirs.toString().split("/");
            for (int i = 0; i < elems.length(); i++) {
                auto elem = elems.at(i);
                finalPath += "/" + elem;
                if (!elem.isEmpty()) {
                    auto aPath = finalPath;
                    // qDebug() << "CREATING DIR:" << aPath;
                    libssh2_sftp_mkdir(sftp_session, aPath, 0775);
                }
            }
        }

        qDebug() << "Source file changed:" << file;
        qDebug() << "Destination path   :" << fullDestPath;
        qDebug() << "Connected as" << userName + "@" + hostName;

        /* Request a file via SFTP */
        sftp_handle = libssh2_sftp_open(sftp_session, file.toUtf8(), LIBSSH2_FXF_READ, 0644);
        sftp_handle_dest = libssh2_sftp_open(sftp_session, fullDestPath.toUtf8(), LIBSSH2_FXF_READ|LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC, 0644);
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
            fin.close();
        }

        libssh2_sftp_close(sftp_handle_dest);
        libssh2_sftp_close(sftp_handle);
        libssh2_sftp_shutdown(sftp_session);
        qDebug() << "SFTP Transfer Successful.";
        connection.resetBuffer();
    } catch (Exception & e) {
        qDebug() << "Exception caught: " << e.what();
    }
}


void FileWatchersManager::dirChangedSlot(const QString& dir) {
    qDebug() << "Dir changed:" << dir;
    scanDir(QDir(dir));
}


void commit() {
    qDebug() << "Committing:";
}
