/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */


#include "file_watcher.h"


FileWatcher::FileWatcher(const QString& name, FileWatchersManager* manager) {
    manager->addPath(name);

    /* connect hooks to invokers */
    // connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(dirChangedSlot(QString)));

}
