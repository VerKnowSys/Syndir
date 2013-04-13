# Syndir Screenshot Sync - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#

TEMPLATE           = app
SYSTEM_NAME        = $$system(uname)
QMAKE_CC           = clang
QMAKE_CXX          = clang++
DEFINES           += GUI_ENABLED PTSSH_SFTP
QT                += gui
ICON               = images/Synshot.icns

HEADERS           += ../Syndir/syndir.h \
                     ../Syndir/worker_thread.h \
                     synshot.h \
                     notifications.h \
                     synshot_config_widget.h \
                     ../Syndir/file_watchers_manager.h

SOURCES           += ../Syndir/file_watchers_manager.cc \
                     synshot.cc \
                     notifications.cc \
                     ../Syndir/worker_thread.cc \
                     synshot_config_widget.cc

TARGET            = ../Synshot

mac {
  CONFIG          += app_bundle
  LIBS            += -lcrypto -lz ../libPTssh.a
  QMAKE_CXXFLAGS  += -std=c++11 -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -w -arch x86_64
  QMAKE_INFO_PLIST   = Synshot.plist
  QMAKE_LDFLAGS     += -arch x86_64

} else {

  CONFIG          += app_bundle link_pkgconfig
  LIBS            += -lcrypto -lz ../libPTssh.a
  QMAKE_CXXFLAGS  += -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -DDEBUG=true
}

RESOURCES         = Synshot.qrc
FORMS             = Synshot.ui
