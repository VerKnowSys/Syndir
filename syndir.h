/**
 *  @author dmilith
 *
 *   © 2013 - VerKnowSys
 *
 */

#ifndef __SYNDIR_H__
#define __SYNDIR_H__


#include <QtCore>
#include <libssh2.h>
#include <libssh2_sftp.h>


#define APP_VERSION "0.15.0"
#define DEFAULT_SESSION_TIMEOUT 30000L /* 30 seconds */
#define ICON_BACK_TO_IDLE_TIMEOUT 3500
#define SSH_PORT 22
#define MAX_PORT_VALUE 65535
#define MAXBUF 16384
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


void usage();


#endif
