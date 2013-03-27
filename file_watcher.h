/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __FILE_WATCHER_H__
#define __FILE_WATCHER_H__


#include <QtCore>
#include <QObject>
#include <QFileSystemWatcher>

#include "file_watchers_manager.h"
#include "syndir.h"


class FileWatcher: public QFileSystemWatcher {
    Q_OBJECT

    public:
        FileWatcher(const QString& name, FileWatchersManager* manager);


    private:
        QString dataDir;

};


#endif
