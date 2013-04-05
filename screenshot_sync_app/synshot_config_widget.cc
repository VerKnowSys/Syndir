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

    trayIcon->setToolTip(QString("Synshot version v") + APP_VERSION);
}


void ConfigWindow::showConfigurePanel() {
    qDebug() << "Launching configure";

    /* load data to line edit elements and bind actions to "textChanged()" signal */
    settingsWindow->defaultSSHPort->setText(settings.value("ssh_port").toString());
    connect(settingsWindow->defaultSSHPort, SIGNAL(textChanged(QString)), this, SLOT(updateDefaultSSHPort(QString)));

    settingsWindow->notificationSound->setText(settings.value("sound_file").toString());
    connect(settingsWindow->notificationSound, SIGNAL(textChanged(QString)), this, SLOT(updateNotificationSound(QString)));

    settingsWindow->screenshotDir->setText(settings.value("source_dir").toString());
    connect(settingsWindow->screenshotDir, SIGNAL(textChanged(QString)), this, SLOT(updateScreenshotDir(QString)));

    settingsWindow->destinationRemoteDir->setText(settings.value("destination_dir").toString());
    connect(settingsWindow->destinationRemoteDir, SIGNAL(textChanged(QString)), this, SLOT(updateDestinationRemoteDir(QString)));

    settingsWindow->urlPrefix->setText(settings.value("remote_path").toString());
    connect(settingsWindow->urlPrefix, SIGNAL(textChanged(QString)), this, SLOT(updateUrlPrefix(QString)));

    this->show();

}


void ConfigWindow::updateDefaultSSHPort(const QString& text) {
    qDebug() << "ssh_port changed to:" << text;
    settings.setValue("ssh_port", text);
}


void ConfigWindow::updateNotificationSound(const QString& text) {
    qDebug() << "sound_file changed to:" << text;
    settings.setValue("sound_file", text);
}


void ConfigWindow::updateScreenshotDir(const QString& text) {
    qDebug() << "source_dir changed to:" << text;
    settings.setValue("source_dir", text);
}


void ConfigWindow::updateDestinationRemoteDir(const QString& text) {
    qDebug() << "destination_dir changed to:" << text;
    settings.setValue("destination_dir", text);
}


void ConfigWindow::updateUrlPrefix(const QString& text) {
    qDebug() << "remote_path changed to:" << text;
    settings.setValue("remote_path", text);
}


void ConfigWindow::closeEvent(QCloseEvent *event) {
    qDebug() << "Got close window event";
    this->hide();
    event->ignore();
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
