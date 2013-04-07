/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "worker_thread.h"


#ifdef GUI_ENABLED
    WorkerThread::WorkerThread(ConfigWindow *config, const QString& sourceDir, const QString& fullDestinationSSHPath, bool screenshotter) {
        this->source = sourceDir;
        this->destination = fullDestinationSSHPath;
        this->screenshotSyncApp = screenshotter;
        this->configWindow = config;
    }
#else
    WorkerThread::WorkerThread(const QString& sourceDir, const QString& fullDestinationSSHPath, bool screenshotter) {
        this->source = sourceDir;
        this->destination = fullDestinationSSHPath;
        this->screenshotSyncApp = screenshotter;
    }
#endif


void WorkerThread::run() {
    this->watcher = new FileWatchersManager(source, destination, screenshotSyncApp);
    #ifdef GUI_ENABLED
        this->watcher->setConfigWindow(this->configWindow);
    #endif
    exec();
}