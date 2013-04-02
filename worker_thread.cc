/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "worker_thread.h"


WorkerThread::WorkerThread(const QString& sourceDir, const QString& fullDestinationSSHPath) {
    this->source = sourceDir;
    this->destination = fullDestinationSSHPath;
}


void WorkerThread::run() {
    this->watcher = new FileWatchersManager(source, destination);
    exec();
}