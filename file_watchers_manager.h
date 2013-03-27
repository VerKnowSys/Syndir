/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __FILE_WATCHERS_MANAGER_H__
#define __FILE_WATCHERS_MANAGER_H__


#include <QtCore>

#include "syndir.h"
// #include "file_watcher.h"
#include "file_watchers_manager.h"


class FileWatchersManager: public QFileSystemWatcher {
    Q_OBJECT

    public:
        FileWatchersManager(const QString& sourceDir, const QString& fullDestinationSSHPath);
        const QStack<QString>* scanDir(QDir dir);


    private:
        const QStack<QString> *files;
        QString fullDestinationSSHPath;
        QString remotePath;
        QString userName;
        QString hostName;


    public slots:
        void fileChangedSlot(const QString& file);
        void dirChangedSlot(const QString& dir);

};


#endif