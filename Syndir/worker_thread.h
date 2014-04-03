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
    #include "../Synshot/synshot_config_widget.h"
#endif


class WorkerThread: public QThread {
    Q_OBJECT

    public:
        #ifdef GUI_ENABLED
            WorkerThread(QPointer<ConfigWindow> config, const QString& sourceDir, const QString& fullDestinationSSHPath);
        #else
            WorkerThread(const QString& sourceDir, const QString& fullDestinationSSHPath);
        #endif
        void run();


    private:
        QPointer<FileWatchersManager> watcher;
        #ifdef GUI_ENABLED
            QPointer<ConfigWindow> configWindow;
        #endif

        QString source;
        QString destination;

};


#endif
