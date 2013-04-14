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
    QProcess *process = new QProcess();
    process->start(OSA_SCRIPT, processArguments);
    process->write(notificationContent.toUtf8());
    process->closeWriteChannel();
    process->waitForFinished();
    delete process;
    logDebug() << "Launched notification:" << notification;
}

#else

void notify(const QString& notification) {

    logDebug() << "Not Yet Implemented";

    logDebug() << "Launched notification:" << notification;
}

#endif
