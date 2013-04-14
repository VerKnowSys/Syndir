# Syndir - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#


include(../Common.pro)


HEADERS += file_watchers_manager.h \
           worker_thread.h \
           syndir.h

SOURCES += worker_thread.cc \
           file_watchers_manager.cc \
           syndir.cc


TARGET = ../Syndir
LIBS += -lz -lcrypto ../libPTssh.a ../liblogger.a
