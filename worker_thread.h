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

#ifdef GUI_ENABLED
    #include "screenshot_sync_app/synshot_config_widget.h"
#endif


class WorkerThread: public QThread {
    Q_OBJECT

    public:
        #ifdef GUI_ENABLED
            WorkerThread(ConfigWindow *config, const QString& sourceDir, const QString& fullDestinationSSHPath);
        #else
            WorkerThread(const QString& sourceDir, const QString& fullDestinationSSHPath);
        #endif
        void run();


    private:
        FileWatchersManager* watcher = NULL;
        #ifdef GUI_ENABLED
            ConfigWindow *configWindow = NULL;
        #endif

        QString source;
        QString destination;

};


#endif
