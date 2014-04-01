/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "synshot_config_widget.h"


ConfigWindow::ConfigWindow() {
    logDebug() << "Initiating configuration window";

    iconGroupBox = new QGroupBox("Tray Icon");
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
    #ifdef __APPLE__
        QString notification = QString("Launched Synshot version v") + APP_VERSION;
        notify(notification);
    #endif

    /* setting default widget position to middle top center */
    QDesktopWidget *desk = new QDesktopWidget();
    int x = desk->width() / 2 - this->width() / 2;
    this->move(x, 0);
    delete desk;

    /* load data to line edit elements and bind actions to "textChanged()" signal */
    settingsWindow->defaultSSHPort->setText(settings.value("ssh_port").toString());
    connect(settingsWindow->defaultSSHPort, SIGNAL(textChanged(QString)), this, SLOT(updateDefaultSSHPort(QString)));

    settingsWindow->defaultSSHPassword->setText(settings.value("ssh_password").toString());
    connect(settingsWindow->defaultSSHPassword, SIGNAL(textChanged(QString)), this, SLOT(updateDefaultSSHPassword(QString)));

    // settingsWindow->notificationSound->setText(settings.value("sound_file").toString());
    // connect(settingsWindow->notificationSound, SIGNAL(textChanged(QString)), this, SLOT(updateNotificationSound(QString)));

    settingsWindow->screenshotDir->setText(settings.value("source_dir").toString());
    connect(settingsWindow->screenshotDir, SIGNAL(textChanged(QString)), this, SLOT(updateScreenshotDir(QString)));

    settingsWindow->destinationRemoteDir->setText(settings.value("destination_dir").toString());
    connect(settingsWindow->destinationRemoteDir, SIGNAL(textChanged(QString)), this, SLOT(updateDestinationRemoteDir(QString)));

    settingsWindow->urlPrefix->setText(settings.value("remote_path").toString());
    connect(settingsWindow->urlPrefix, SIGNAL(textChanged(QString)), this, SLOT(updateUrlPrefix(QString)));

    settingsWindow->allowedFileTypes->setPlainText(settings.value("allowed_file_types").toString());
    connect(settingsWindow->allowedFileTypes, SIGNAL(textChanged()), this, SLOT(updateAllowedFileTypes()));
}


void ConfigWindow::backToIdleState() {
    logDebug() << "Back to idle.";
    trayIcon->setIcon(iconDefault);
}


void ConfigWindow::doingWork(int state) {
    logDebug() << "Got state:" << state;
    switch (state) {
        case IDLE: {
            trayIcon->setIcon(iconDefault);
        } break;

        case WORKING: {
            trayIcon->setIcon(iconWorking);
        } break;

        case OK: {
            trayIcon->setIcon(iconOk);
        } break;

        case ERROR: {
            trayIcon->setIcon(iconError);
        } break;

        case DELETE: {
            trayIcon->setIcon(iconDelete);
        } break;
    }
    if (state == OK)
        QTimer::singleShot(ICON_BACK_TO_IDLE_TIMEOUT, this, SLOT(backToIdleState()));
}


void ConfigWindow::showConfigurePanel() {
    logDebug() << "Showing configure panel";
    this->show();
}


void ConfigWindow::updateDefaultSSHPassword(const QString& text) {
    if (text.length() > 0) {
        settingsWindow->defaultSSHPassword->setStyleSheet("border: 2px solid green");
        settings.setValue("ssh_password", text);
        logDebug() << "ssh_password changed";
    }
    if (text.length() == 0) {
        settingsWindow->defaultSSHPassword->setStyleSheet("border: 2px solid green");
        settings.setValue("ssh_password", SSH_PASSWORD); /* set default value if entry is empty -> means that we don't want to use ssh password for remote access */
        logDebug() << "Password was reset. SSH auth wont be used";
    }
}


void ConfigWindow::updateDefaultSSHPort(const QString& text) {
    bool ok = false;
    uint port = text.toInt(&ok, 10);
    if (ok and (port <= MAX_PORT_VALUE)) {
        settingsWindow->defaultSSHPort->setStyleSheet("border: 2px solid green");
        settings.setValue("ssh_port", text);
        logDebug() << "ssh_port changed to:" << text;
    } else {
        settingsWindow->defaultSSHPort->setStyleSheet("border: 2px solid red");
    }
}


// void ConfigWindow::updateNotificationSound(const QString& text) {
//     if (QFile::exists(text)) {
//         settingsWindow->notificationSound->setStyleSheet("border: 2px solid green");
//         settings.setValue("sound_file", text);
//         logDebug() << "sound_file changed to:" << text;
//     } else {
//         settingsWindow->notificationSound->setStyleSheet("border: 2px solid red");
//     }
// }


void ConfigWindow::updateScreenshotDir(const QString& text) {
    QString destText = text;
    destText = destText.replace(QChar('~'), getenv("HOME")); /* replace ~ special sign to user HOME dir */
    if (QDir(destText).exists()) {
        settingsWindow->screenshotDir->setStyleSheet("border: 2px solid green");
        settings.setValue("source_dir", text);
        logDebug() << "source_dir changed to:" << text;
    } else {
        settingsWindow->screenshotDir->setStyleSheet("border: 2px solid red");
    }
}


void ConfigWindow::updateDestinationRemoteDir(const QString& text) {
    // QString destText = text;
    // destText = destText.replace(QChar('~'), getenv("HOME")); /* replace ~ special sign to user HOME dir, NOTE: it wont use HOME dir of remote system but local system home dir. Use wisely. */
    // if (QDir(destText).exists()) {
    settingsWindow->destinationRemoteDir->setStyleSheet("border: 2px solid green");
    settings.setValue("destination_dir", text);
    logDebug() << "destination_dir changed to:" << text;
    // } else {
    //     settingsWindow->destinationRemoteDir->setStyleSheet("border: 2px solid red");
    // }
}


void ConfigWindow::updateUrlPrefix(const QString& text) {
    /* don't store same value */
    if (text != settings.value("remote_path").toString()) {
        settingsWindow->urlPrefix->setStyleSheet("border: 2px solid green");
        settings.setValue("remote_path", text);
        logDebug() << "remote_path changed to:" << text;
    }
}


void ConfigWindow::updateAllowedFileTypes() {
    if (settingsWindow->allowedFileTypes->toPlainText() != settings.value("allowed_file_types").toString()) {
        QString text = settingsWindow->allowedFileTypes->toPlainText();
        settingsWindow->allowedFileTypes->setStyleSheet("border: 2px solid green");
        if (text.length() == 0) {
            text = ALLOWED_FILE_TYPES;
        }
        settings.setValue("allowed_file_types", text);
        logDebug() << "allowed_file_types changed to:" << text;
    }
}


void ConfigWindow::closeEvent(QCloseEvent *event) {
    logDebug() << "Got close window event";
    this->hide();
    event->ignore();
}


void ConfigWindow::keyPressEvent(QKeyEvent *event) {
    logDebug() << "Got key event";
    if (event->key() == Qt::Key_Escape) {
        this->hide();
        event->ignore();
    }
}


void ConfigWindow::createTrayIcon() {
    trayIconMenu = new QMenu(this);
    // trayIconMenu->setIcon(icon);
    trayIconMenu->addAction(configureAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(iconDefault);
}
