/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */


#include "file_watcher.h"


FileWatcher::FileWatcher(const QString& name, FileWatchersManager* manager) {
    this->dataDir = name;
    manager->addPath(name);


    /* connect hooks to invokers */
    // connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(dirChangedSlot(QString)));
    connect(this, SIGNAL(fileChanged(QString)), manager, SLOT(fileChangedSlot(QString)));
}


// void FileWatcher::dirChangedSlot(const QString& dir) {
//     if (QFileInfo(dir).isDir()) {
//         qDebug() << "Found modification in directory:" << dir;
//         qDebug() << "-> Reaction (TODO):" << "reloading of file watchers for this directory";
//     } else {
//         qDebug() << "Accessing remote server for a file:" << dir;
//         qDebug() << "-> Reaction (TODO):" << "sending differences to remote";
//     }
// }

