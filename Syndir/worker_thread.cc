/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "worker_thread.h"


#ifdef GUI_ENABLED
    WorkerThread::WorkerThread(ConfigWindow *config, const QString& sourceDir, const QString& fullDestinationSSHPath) {
        this->source = sourceDir;
        this->destination = fullDestinationSSHPath;
        this->configWindow = config;
    }
#else
    WorkerThread::WorkerThread(const QString& sourceDir, const QString& fullDestinationSSHPath) {
        this->source = sourceDir;
        this->destination = fullDestinationSSHPath;
    }
#endif


void WorkerThread::run() {
    this->watcher = new FileWatchersManager(source, destination); /* true => store files on remote as SHA1 of original name */
    #ifdef GUI_ENABLED
        this->watcher->setConfigWindow(this->configWindow);
    #endif
    exec();
}
