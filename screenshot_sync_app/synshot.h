/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __SYNSHOT_H__
#define __SYNSHOT_H__

#include <QtGui/QtGui>

#include "../syndir.h"
#include "../worker_thread.h"
#include "synshot_config_widget.h"
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

#define DEFAULT_SOUND_FILE "/System/Library/Sounds/Hero.aiff"
#define DEFAULT_SOURCE_DIR "~/Pictures/ScreenShots"
#define DEFAULT_DESTINATION_DIR "v:/home/dmilith/Web/Public/Sshots"
#define REMOTE_PATH "http://s.verknowsys.com/"


#endif
