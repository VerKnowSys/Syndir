/**
 *  @author dmilith, tallica
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __FILE_WATCHER_H__
#define __FILE_WATCHER_H__


#include <QtCore>
#include <QObject>
#include <QFile>
#include <QEventLoop>
#include <QFileSystemWatcher>


class FileWatcher: public QFileSystemWatcher {
    Q_OBJECT

    public:
        FileWatcher(const QString& name, const QString& remoteSSHPath);

    private:
        QString dataDir;
        QString remotePath;
        QString userName;
        QString hostName;

    signals:

    public slots:
        // void shutdownSlot();
        void dirChangedSlot(const QString& dir);

};


#endif
