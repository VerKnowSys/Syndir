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
#include <sys/stat.h>

#ifdef __linux__
    #include <unistd.h>
#endif

#include "syndir.h"

#ifdef GUI_ENABLED
    #include "../Synshot/synshot_config_widget.h"
#endif


using namespace std;


class FileWatchersManager: public QFileSystemWatcher {
    Q_OBJECT

    public:
        FileWatchersManager(const QString& sourceDir, const QString& fullDestinationSSHPath);
        void callSuccess();
        void scanDir(QDir dir);
        void copyFilesToRemoteHost(const QStringList& fileList, bool hashFile = false);
        QStringList removeFromList(QStringList& list, const QStringList& toDelete);
        void executeRemoteCommand(const QString& command);
        bool sendFileToRemote(PTssh* connection, const QString& file, const QString& destinationFile);
        void loadSettings(); /* load app settings */


        #ifdef GUI_ENABLED
            void setConfigWindow(QPointer<ConfigWindow> config);
        #endif


    signals:
        /* slot to trigger icon type in tray */
        void setWork(int state);


    protected:
        const QString keysLocation = QString(getenv("HOME")) + "/.ssh";


    private:
        QStringList files = QStringList(), oldFiles = QStringList();
        QString baseCWD, fullDestinationSSHPath, remotePath, userName, hostName;
        int sshPort = SSH_PORT;

        QString last = "NOFILE";
        QDateTime lastModified;

        QSettings settings;
        PTssh *connection = NULL;

        #ifdef GUI_ENABLED
            QPointer<ConfigWindow> configWindow;
        #endif


    public slots:
        void disconnectSSHSession();
        void connectToRemoteHost();
        void fileChangedSlot(const QString& file);
        void dirChangedSlot(const QString& dir);

};

#endif