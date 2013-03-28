/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __FILE_WATCHERS_MANAGER_H__
#define __FILE_WATCHERS_MANAGER_H__


#include <iostream>
#include <fstream>
#include <string>
#include <iostream>

#include "syndir.h"
#include "file_watchers_manager.h"


using namespace std;


class FileWatchersManager: public QFileSystemWatcher {
    Q_OBJECT

    public:
        FileWatchersManager(const QString& sourceDir, const QString& fullDestinationSSHPath);
        void scanDir(QDir dir);
        void copyFileToRemoteHost(const QString& file);


    private:
        QString baseCWD;
        QStringList files;
        QString fullDestinationSSHPath;
        QString remotePath;
        QString userName;
        QString hostName;


    public slots:
        void fileChangedSlot(const QString& file);
        void dirChangedSlot(const QString& dir);

};


#endif