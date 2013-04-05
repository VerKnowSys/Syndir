/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "synshot_config_widget.h"


ConfigWindow::ConfigWindow() {
    iconGroupBox = new QGroupBox(tr("Tray Icon"));
    iconLabel = new QLabel("Icon:");
    iconLabel->setMinimumWidth(32);

    /* define actions */
    configureAction = new QAction(tr("&Configure"), this);
    connect(configureAction, SIGNAL(triggered()), this, SLOT(showConfigurePanel()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    /* end actions */

    createTrayIcon();
    trayIcon->show();
    this->setAttribute(Qt::WA_DeleteOnClose); // just remove widget

    settingsWindow = new ConfigWidget();
    settingsWindow->setupUi(this);
}


void ConfigWindow::showConfigurePanel() {
    qDebug() << "Launching configure";

    /* load data to line edit elements */
    settingsWindow->defaultSSHPort->setText("1234");
    settingsWindow->notificationSound->setText("asd");
    settingsWindow->screenshotDir->setText("fds");
    settingsWindow->destinationRemoteDir->setText("dvs");
    settingsWindow->urlPrefix->setText("t4eb");
    this->show();

}


void ConfigWindow::createTrayIcon() {
    QIcon icon = QIcon(":/images/find.tiff");

    trayIconMenu = new QMenu(this);
    trayIconMenu->setIcon(icon);
    // trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(configureAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    trayIcon->setIcon(icon);
}
