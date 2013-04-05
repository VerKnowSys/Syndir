/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __SYNSHOT_CONF_WIDGET_H__
#define __SYNSHOT_CONF_WIDGET_H__

#include "synshot.h"

/* auto generated: */
#include "ui_Synshot.h"

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
        void updateNotificationSound(const QString& text);
        void updateScreenshotDir(const QString& text);
        void updateDestinationRemoteDir(const QString& text);
        void updateUrlPrefix(const QString& text);


    private:
        QAction *quitAction, *configureAction;

        QSettings settings;
        ConfigWidget *settingsWindow;

        QGroupBox *iconGroupBox;
        QLabel *iconLabel;
        QMenu *trayIconMenu;
        QSystemTrayIcon *trayIcon;
};

#endif