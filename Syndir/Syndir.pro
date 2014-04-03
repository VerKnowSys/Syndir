# Syndir - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#


include(../Common.pro)

OBJECTS_DIR = .obj
MOC_DIR = .moc


HEADERS += \
            file_watchers_manager.h \
            worker_thread.h \
            syndir.h

SOURCES += \
            file_watchers_manager.cc \
            worker_thread.cc \
            syndir.cc


TARGET = ../Syndir
LIBS += -lz -lcrypto ../libPTssh.a ../liblogger.a
