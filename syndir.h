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
#include "ssh_wrapper/Connection.h"
#include "ssh_wrapper/Exception.h"
#include "ssh_wrapper/UserInfo.h"


using namespace SSH2Wrapper;

#define APP_VERSION "0.3.1"
#define SSH_PORT 22
#define COPYRIGHT "Copyright © 2o13 VerKnowSys.com - All Rights Reserved."
#define DEFAULT_STRING_CODEC "UTF8"


void usage();


#endif
