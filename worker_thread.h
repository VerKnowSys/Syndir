/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __WORKER_THREAD_H__
#define __WORKER_THREAD_H__


#include "syndir.h"
#include "file_watchers_manager.h"


class WorkerThread: public QThread {
    Q_OBJECT

    public:
        WorkerThread(const QString& sourceDir, const QString& fullDestinationSSHPath);
        void run();


    private:
        FileWatchersManager* watcher = NULL;
        QString source;
        QString destination;

};


#endif
