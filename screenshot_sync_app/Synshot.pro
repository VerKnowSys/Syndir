# Syndir Screenshot Sync - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#

TEMPLATE           = app
SYSTEM_NAME        = $$system(uname)
QMAKE_CC           = clang
QMAKE_CXX          = clang++

QT                += gui
ICON               = images/Synshot.icns

HEADERS           += ../syndir.h \
                     synshot.h \
                     synshot_config_widget.h \
                     ../file_watchers_manager.h \
                     ../worker_thread.h \
                     ../ssh_wrapper/Connection.h \
                     ../ssh_wrapper/Exception.h \
                     ../ssh_wrapper/UserInfo.h

SOURCES           += ../file_watchers_manager.cc \
                     synshot.cc \
                     synshot_config_widget.cc \
                     ../worker_thread.cc \
                     ../ssh_wrapper/Connection.cpp \
                     ../ssh_wrapper/Exception.cpp \
                     ../ssh_wrapper/UserInfo.cpp

TARGET            = ../Synshot

mac {
  CONFIG          += app_bundle
  INCLUDEPATH     += ${HOME}/Apps/Libssh2/include
  LIBS            += ${HOME}/Apps/Libssh2/lib/libssh2.${LIBTYPE} -lcrypto -lz
  QMAKE_CXXFLAGS  += -std=c++11 -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -DGUI_ENABLED -w -arch x86_64
  QMAKE_INFO_PLIST   = Synshot.plist
  QMAKE_LDFLAGS     += -arch x86_64

} else {

  CONFIG          += app_bundle link_pkgconfig
  PKGCONFIG        = libssh2
  LIBS            += -lcrypto -lz
  QMAKE_CXXFLAGS  += -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -DGUI_ENABLED -DDEBUG=true
}

RESOURCES         = Synshot.qrc
FORMS             = Synshot.ui
