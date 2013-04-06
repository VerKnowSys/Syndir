/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __NOTIFICATIONS_H__
#define __NOTIFICATIONS_H__


#include <QtCore>
#include <QProcess>


#ifdef __APPLE__
    const QString OSA_SCRIPT("/usr/bin/osascript");
    const QString GROWL_APPLESCRIPT(
        "tell application \"System Events\"\n"
        "  set isRunning to (count of (every process whose bundle identifier is \"com.Growl.GrowlHelperApp\")) > 0\n"
        "end tell\n"
        "if isRunning then\n"
          "tell application id \"com.Growl.GrowlHelperApp\"\n"
            "set the allNotificationsList to {\"Synshot Notification\", \"Synshot Notification\"}\n"
            "set the enabledNotificationsList to {\"Synshot Notification\"}\n"
            "register as application \"Synshot\" all notifications allNotificationsList default notifications enabledNotificationsList icon of application \"Synshot\"\n"
            "notify with name \"Synshot Notification\" title \"Synshot Notification\" description \"NOTIFICATION_CONTENTS\" application name \"Synshot\"\n"
            "notify with name \"Another Synshot Notification\" title \"Another Synshot Notification\" description \"NOTIFICATION_CONTENTS\" application name \"Synshot\"\n"
          "end tell\n"
        "end if\n");

#endif


void notify(const QString& notification);


#endif
