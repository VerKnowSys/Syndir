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

HEADERS += \
          ../Syndir/worker_thread.h \
          ../Syndir/file_watchers_manager.h \
          notifications.h \
          synshot_config_widget.h \
          synshot.h

SOURCES += \
          ../Syndir/worker_thread.cc \
          ../Syndir/file_watchers_manager.cc \
          notifications.cc \
          synshot_config_widget.cc \
          synshot.cc

RESOURCES = Synshot.qrc
FORMS = Synshot.ui
TARGET = ../Synshot

LIBS += -lcrypto -lz ../libPTssh.a ../liblogger.a

mac {

  CONFIG += app_bundle
  QMAKE_INFO_PLIST = Synshot.plist

}
