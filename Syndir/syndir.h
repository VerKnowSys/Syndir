/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __SYNDIR_H__
#define __SYNDIR_H__


/* ignore certain kinds of warnings: */
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wmismatched-tags"


/* all system wide includes required: */
#include <QtCore>
#include <sys/stat.h>

#include <QCryptographicHash>

#include "../CuteLogger/Logger.h"
#include "../CuteLogger/ConsoleAppender.h"
#include "../CuteLogger/FileAppender.h"

#include "../PTssh/PTsshConfig.h"
#include "../PTssh/PTsshW.h"
#include "../PTssh/PTssh.h"
#include "../PTssh/PTSftp.h"


#define APP_VERSION "0.18.2"


#define DEFAULT_CONNECTION_TIMEOUT 10000L /* 10 seconds */
#define ICON_BACK_TO_IDLE_TIMEOUT 3500
#define SSH_PORT 22
#define SSH_PASSWORD "NO-SSH-PASSWORD-SET"
#define MAX_PORT_VALUE 65535
#define COPYRIGHT "Copyright © 2o13 VerKnowSys.com - All Rights Reserved."
#define DEFAULT_STRING_CODEC "UTF8"
#define ALLOWED_FILE_TYPES "\\.pro$|\\.hpp$|\\.h$|\\.c$|\\.cc$|\\.cpp$|\\.scala$|\\.java$|\\.rb$|Rakefile$|\\.properties$|\\.xml$|\\.html$|\\.js$|\\.coffee$|\\.sh$|\\.def$|\\.md$|\\.haml$|\\.slim$|\\.css$|\\.sass$|\\.scss$|\\.less$|\\.erb$|\\.yml|\\.yaml$$|\\.conf$|\\.json$|\\.jpg$|\\.jpeg$|\\.png$|\\.svg$|\\.gif$|\\.jar$|\\.ini$|\\.patch$|\\.jp2$"

#define DEFAULT_SOUND_FILE "/System/Library/Sounds/Hero.aiff"
#define DEFAULT_SOURCE_DIR "~/Pictures/ScreenShots"
#define DEFAULT_DESTINATION_DIR "v:/home/dmilith/Web/Public/Sshots"
#define REMOTE_PATH "http://s.verknowsys.com/"


/* State */
#define IDLE 0
#define WORKING 1
#define OK 2
#define ERROR 3
#define DELETE 4


/* Loggers */
#define logTrace LOG_TRACE
#define logDebug LOG_DEBUG
#define logInfo  LOG_INFO
#define logWarn  LOG_WARNING
#define logError LOG_ERROR
#define logFatal LOG_FATAL



void usage();


#endif