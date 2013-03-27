/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __SYNDIR_H__
#define __SYNDIR_H__

#include <QtCore>
#include <QObject>
#include <QFile>
#include <QEventLoop>
#include <QFileSystemWatcher>


class FileWatcher: public QFileSystemWatcher {
    Q_OBJECT

    public:
        FileWatcher(const QString& name);
        // ~SvdServiceWatcher();

    private:
        QString dataDir;

        // void cleanupTriggerHookFiles();

    signals:
        // void installService();
        // void configureService();
        // void validateService();
        // void startService();
        // void afterStartService();
        // void stopService();
        // void afterStopService();
        // void restartService();
        // void reloadService();

    public slots:
        // void shutdownSlot();
        void dirChangedSlot(const QString& dir);
        // void fileChangedSlot(const QString& file);

};


#endif
