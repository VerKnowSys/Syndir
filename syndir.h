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

#include "file_watchers_manager.h"

#define DEFAULT_SESSION_TIMEOUT 30000L /* 30 seconds */
#define SSH_PORT 22
#define MAX_PORT_VALUE 65535
#define APP_VERSION "0.12.2"
#define MAXBUF 12288
#define COPYRIGHT "Copyright © 2o13 VerKnowSys.com - All Rights Reserved."
#define DEFAULT_STRING_CODEC "UTF8"
#define ALLOWED_FILE_TYPES "\\.pro$|\\.hpp$|\\.h$|\\.c$|\\.cc$|\\.cpp$|\\.scala$|\\.java$|\\.rb$|Rakefile$|\\.properties$|\\.xml$|\\.html$|\\.js$|\\.coffee$|\\.sh$|\\.def$|\\.md$|\\.haml$|\\.slim$|\\.css$|\\.sass$|\\.scss$|\\.less$|\\.erb$|\\.yml|\\.yaml$$|\\.conf$|\\.json$|\\.jpg$|\\.jpeg$|\\.png$|\\.svg$|\\.gif$|\\.jar$|\\.ini$|\\.patch$|\\.jp2$"

void usage();


#endif
