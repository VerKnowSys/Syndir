# Syndir Screenshot Sync - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#

mac {
  CONFIG          += app_bundle
} else {
  CONFIG          += app_bundle link_pkgconfig
  PKGCONFIG        = libssh2
}
TEMPLATE           = app
SYSTEM_NAME        = $$system(uname)
QMAKE_CXX          = clang++
QT                += gui

HEADERS           += ../syndir.h \
                     synshot.h \
                     ../file_watchers_manager.h \
                     ../worker_thread.h \
                     ../ssh_wrapper/Connection.h \
                     ../ssh_wrapper/Exception.h \
                     ../ssh_wrapper/UserInfo.h

SOURCES           += ../file_watchers_manager.cc \
                     synshot.cc \
                     ../worker_thread.cc \
                     ../ssh_wrapper/Connection.cpp \
                     ../ssh_wrapper/Exception.cpp \
                     ../ssh_wrapper/UserInfo.cpp

# TARGET            = ../synshot

mac {
  INCLUDEPATH     += ${HOME}/Apps/Libssh2/include
  LIBS            += ${HOME}/Apps/Libssh2/lib/libssh2.${LIBTYPE} -lcrypto -lz
} else {
  LIBS            += -lcrypto -lz
}

contains(SYSTEM_NAME, Linux): {
  QMAKE_CXXFLAGS  += -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -DDEBUG=true
} else {
  QMAKE_CXXFLAGS  += -std=c++11 -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -DGUI_ENABLED -w
  # -DDEBUG=true
}

mac {
  QMAKE_INFO_PLIST   = Synshot.plist
}

QMAKE_LDFLAGS     += -DGUI_ENABLED
