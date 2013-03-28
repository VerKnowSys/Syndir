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

#include "ssh_wrapper/Connection.h"
#include "ssh_wrapper/Exception.h"
#include "ssh_wrapper/UserInfo.h"

#include "syndir.h"
#include "file_watchers_manager.h"


using namespace SSH2Wrapper;
using namespace std;


class FileWatchersManager: public QFileSystemWatcher {
    Q_OBJECT

    public:
        FileWatchersManager(const QString& sourceDir, const QString& fullDestinationSSHPath);
        ~FileWatchersManager();
        void scanDir(QDir dir);
        void copyFileToRemoteHost(const QString& file);


    private:
        QString baseCWD;
        QStringList files;
        QString fullDestinationSSHPath;
        QString remotePath;
        QString userName;
        QString hostName;

        Connection* connection = NULL;
        LIBSSH2_SFTP *sftp_session = NULL;
        LIBSSH2_SFTP_HANDLE *sftp_handle = NULL;
        LIBSSH2_SFTP_HANDLE *sftp_handle_dest = NULL;


    public slots:
        void fileChangedSlot(const QString& file);
        void dirChangedSlot(const QString& dir);

};


#endif