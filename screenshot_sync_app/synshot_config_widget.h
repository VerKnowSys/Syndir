/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __SYNSHOT_CONF_WIDGET_H__
#define __SYNSHOT_CONF_WIDGET_H__

#include "synshot.h"
#include "ui_Synshot.h"


using namespace Ui;


class ConfigWindow: public QDialog {
    Q_OBJECT

    public:
        ConfigWindow();
        void createTrayIcon();

    public slots:
        void showConfigurePanel();


    private:
        QAction *quitAction;
        QAction *configureAction;

        ConfigWidget *settingsWindow;
        QGroupBox *iconGroupBox;
        QLabel *iconLabel;
        QMenu *trayIconMenu;
        QSystemTrayIcon *trayIcon;
};

#endif