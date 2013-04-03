/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "worker_thread.h"


WorkerThread::WorkerThread(const QString& sourceDir, const QString& fullDestinationSSHPath, bool screenshotter) {
    this->source = sourceDir;
    this->destination = fullDestinationSSHPath;
    this->screenshotSyncApp = screenshotter;
}


void WorkerThread::run() {
    this->watcher = new FileWatchersManager(source, destination, screenshotSyncApp);
    exec();
}