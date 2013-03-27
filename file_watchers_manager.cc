/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "syndir.h"


FileWatchersManager::FileWatchersManager(const QString& sourceDir, const QString& fullDestinationSSHPath) {
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

    scanDir(QDir(sourceDir)); /* will fill up manager 'files' field */
    qDebug() << "Total files:" << files.size();
    qDebug() << "Traversing paths";

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

    addPaths(files);

    /* connect hooks to invokers */
    connect(this, SIGNAL(fileChanged(QString)), this, SLOT(fileChangedSlot(QString)));
    connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(dirChangedSlot(QString)));
}


void FileWatchersManager::fileChangedSlot(const QString& file) {
    qDebug() << "File changed:" << file;

}


void FileWatchersManager::dirChangedSlot(const QString& dir) {
    qDebug() << "Dir changed:" << dir;
    scanDir(QDir(dir));
}


void commit() {
    qDebug() << "Commiting:";
}
