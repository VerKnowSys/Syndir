/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __SYNSHOT_CONF_WIDGET_H__
#define __SYNSHOT_CONF_WIDGET_H__

#include <QtGui/QtGui>

/* auto generated: */
#include "ui_Synshot.h"

#include "../Syndir/syndir.h"
#include "notifications.h"


using namespace Ui;


class ConfigWindow: public QDialog {
    Q_OBJECT

    public:
        ConfigWindow();
        void createTrayIcon();

    public slots:
        void showConfigurePanel();
        void closeEvent(QCloseEvent *event);
        void keyPressEvent(QKeyEvent *event);

        void updateDefaultSSHPort(const QString& text);
        void updateDefaultSSHPassword(const QString& text);
        // void updateNotificationSound(const QString& text);
        void updateScreenshotDir(const QString& text);
        void updateDestinationRemoteDir(const QString& text);
        void updateUrlPrefix(const QString& text);
        void updateAllowedFileTypes();

        void doingWork(int state); /* will invoke icon set */
        void backToIdleState(); /* will set icon back to idle state */


    private:
        const QIcon iconDefault = QIcon(":/images/default.tiff");
        const QIcon iconWorking = QIcon(":/images/working.tiff");
        const QIcon iconOk      = QIcon(":/images/ok.tiff");
        const QIcon iconError   = QIcon(":/images/error.tiff");
        const QIcon iconDelete  = QIcon(":/images/delete.tiff");

        QSettings settings;
        QAction *quitAction = NULL, *configureAction = NULL;
        ConfigWidget *settingsWindow = NULL;
        QGroupBox *iconGroupBox = NULL;
        QLabel *iconLabel = NULL;
        QMenu *trayIconMenu = NULL;
        QSystemTrayIcon *trayIcon = NULL;
};


#endif