/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#include "notifications.h"


#ifdef __APPLE__

void notify(const QString& notification) {
    QString tmpl = GROWL_APPLESCRIPT;
    QString notificationContent = tmpl.replace("NOTIFICATION_CONTENTS", notification);
    QStringList processArguments;
    processArguments << "-l" << "AppleScript";
    QProcess process;
    process.start(OSA_SCRIPT, processArguments);
    process.write(notificationContent.toUtf8());
    process.closeWriteChannel();
    process.waitForFinished();
    qDebug() << "Launched notification:" << notification;
}

#else

void notify(const QString& notification) {

    qDebug() << "Not Yet Implemented";

    qDebug() << "Launched notification:" << notification;
}

#endif
