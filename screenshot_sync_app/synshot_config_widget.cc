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
    bool ok = false;
    uint pid = text.toInt(&ok, 10);
    if (ok and (pid < 65536)) {
        settingsWindow->defaultSSHPort->setStyleSheet("border: 2px solid green");
        settings.setValue("ssh_port", text);
        qDebug() << "ssh_port changed to:" << text;
    } else {
        settingsWindow->defaultSSHPort->setStyleSheet("border: 2px solid red");
    }
}


void ConfigWindow::updateNotificationSound(const QString& text) {
    if (QFile::exists(text)) {
        settingsWindow->notificationSound->setStyleSheet("border: 2px solid green");
        settings.setValue("sound_file", text);
        qDebug() << "sound_file changed to:" << text;
    } else {
        settingsWindow->notificationSound->setStyleSheet("border: 2px solid red");
    }
}


void ConfigWindow::updateScreenshotDir(const QString& text) {
    QString destText = text;
    destText = destText.replace(QChar('~'), getenv("HOME")); /* replace ~ special sign to user HOME dir */
    if (QDir(destText).exists()) {
        settingsWindow->screenshotDir->setStyleSheet("border: 2px solid green");
        settings.setValue("source_dir", text);
        qDebug() << "source_dir changed to:" << text;
    } else {
        settingsWindow->screenshotDir->setStyleSheet("border: 2px solid red");
    }
}


void ConfigWindow::updateDestinationRemoteDir(const QString& text) {
    QString destText = text;
    destText = destText.replace(QChar('~'), getenv("HOME")); /* replace ~ special sign to user HOME dir, NOTE: it wont use HOME dir of remote system but local system home dir. Use wisely. */
    if (QDir(destText).exists()) {
        settingsWindow->destinationRemoteDir->setStyleSheet("border: 2px solid green");
        settings.setValue("destination_dir", text);
        qDebug() << "destination_dir changed to:" << text;
    } else {
        settingsWindow->destinationRemoteDir->setStyleSheet("border: 2px solid red");
    }
}


void ConfigWindow::updateUrlPrefix(const QString& text) {
    settingsWindow->urlPrefix->setStyleSheet("border: 2px solid green");
    settings.setValue("remote_path", text);
    qDebug() << "remote_path changed to:" << text;
}


void ConfigWindow::closeEvent(QCloseEvent *event) {
    qDebug() << "Got close window event";
    this->hide();
    event->ignore();
}


void ConfigWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << "Got key event";
    if (event->key() == Qt::Key_Escape) {
        this->hide();
        event->ignore();
    }
}


void ConfigWindow::createTrayIcon() {
    QIcon icon = QIcon(":/images/find.tiff");

    trayIconMenu = new QMenu(this);
    trayIconMenu->setIcon(icon);
    trayIconMenu->addAction(configureAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(icon);
}
