/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */


#include "file_watcher.h"


FileWatcher::FileWatcher(const QString& name) {
    dataDir = name;
    connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(dirChangedSlot(QString)));
}

void FileWatcher::dirChangedSlot(const QString& dir) {
    qDebug() << "Directory changed:" << dir;
}
