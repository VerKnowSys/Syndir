/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */


#include "syndir.h"
#include "file_watcher.h"


FileWatcher::FileWatcher(const QString& name, const QString& remoteSSHPath) {
    this->dataDir = name;
    this->addPath(name);
    QStringList sshDirPartial = remoteSSHPath.split(":");
    QStringList sshUserServerPartial = sshDirPartial.value(0).split("@"); /* first part is user@host */
    this->remotePath = sshDirPartial.value(1); /* last one is a remote path */
    this->userName = sshUserServerPartial.value(0);
    this->hostName = sshUserServerPartial.value(1);

    // qDebug() << "Remote setup inspection:\nRemote user:" << userName << "\nRemote host:" << hostName << "\nRemote path:" << remotePath;

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
    connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(dirChangedSlot(QString)));
}


void FileWatcher::dirChangedSlot(const QString& dir) {
    QString post = "";
    if (QFileInfo(dir).isDir()) {
        qDebug() << "Found modification in directory:" << dir;
        qDebug() << "-> Reaction (TODO):" << "reloading of file watchers for this directory";
    } else {
        qDebug() << "Accessing remote server for a file:" << dir;
        qDebug() << "-> Reaction (TODO):" << "sending differences to remote";
    }
}


// void FileWatcher::shutdownSlot() {
//     qDebug() << "Shutting down";
// }
