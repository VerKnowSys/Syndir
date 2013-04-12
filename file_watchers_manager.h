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

#include "syndir.h"

#ifdef GUI_ENABLED
    #include "screenshot_sync_app/synshot_config_widget.h"
#endif


using namespace std;


class FileWatchersManager: public QFileSystemWatcher {
    Q_OBJECT

    public:
        FileWatchersManager(const QString& sourceDir, const QString& fullDestinationSSHPath);
        void scanDir(QDir dir);
        void copyFileToRemoteHost(const QString& file, bool hashFile = false);
        void connectToRemoteHost();
        QStringList removeFromList(QStringList& list, const QStringList& toDelete);
        void executeRemoteCommand(const QString& command);
        void loadSettings(); /* load app settings */


        #ifdef GUI_ENABLED
            void setConfigWindow(ConfigWindow *config);
        #endif


    signals:
        /* slot to trigger icon type in tray */
        void setWork(int state);


    protected:
        const QString keysLocation = QString(getenv("HOME")) + "/.ssh";


    private:
        QStringList files = QStringList(), oldFiles = QStringList();
        QString baseCWD, fullDestinationSSHPath, remotePath, userName, hostName;

        QString last = "NOFILE";
        QDateTime lastModified;

        QSettings settings;
        PTssh *connection = NULL;

        #ifdef GUI_ENABLED
            ConfigWindow *configWindow = NULL;
        #endif


    public slots:
        void fileChangedSlot(const QString& file);
        void dirChangedSlot(const QString& dir);

};

#endif