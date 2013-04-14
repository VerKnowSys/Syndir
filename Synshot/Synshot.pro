# Synshot Screenshot Sync - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#


include(../Common.pro)


TEMPLATE = app
DEFINES += GUI_ENABLED
QT += gui
ICON = images/Synshot.icns

HEADERS += ../Syndir/syndir.h \
           ../Syndir/worker_thread.h \
           synshot.h \
           notifications.h \
           synshot_config_widget.h \
           ../Syndir/file_watchers_manager.h

SOURCES += ../Syndir/file_watchers_manager.cc \
           synshot.cc \
           notifications.cc \
           ../Syndir/worker_thread.cc \
           synshot_config_widget.cc

RESOURCES = Synshot.qrc
FORMS = Synshot.ui
TARGET = ../Synshot

LIBS += -lcrypto -lz ../libPTssh.a ../liblogger.a

mac {

  CONFIG += app_bundle
  QMAKE_INFO_PLIST = Synshot.plist

}
