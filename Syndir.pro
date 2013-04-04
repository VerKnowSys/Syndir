# Syndir - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#

SYSTEM_NAME        = $$system(uname)
CONFIG            += link_pkgconfig
PKGCONFIG          = libssh2 QtCore
QMAKE_CXX          = clang++

QT                -= gui
# QT                += network

HEADERS           += syndir.h \
                     file_watchers_manager.h \
                     worker_thread.h \
                     ssh_wrapper/Connection.h \
                     ssh_wrapper/Exception.h \
                     ssh_wrapper/UserInfo.h

SOURCES           += syndir.cc \
                     worker_thread.cc \
                     ssh_wrapper/Connection.cpp \
                     ssh_wrapper/Exception.cpp \
                     ssh_wrapper/UserInfo.cpp \
                     file_watchers_manager.cc

TARGET             = syndir

LIBS              += -lcrypto -lz

contains(SYSTEM_NAME, Linux): {
  QMAKE_CXXFLAGS  += -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -DDEBUG=true
} else {
  QMAKE_CXXFLAGS  += -std=c++11 -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE
  # -DDEBUG=true
}
# QMAKE_LDFLAGS     +=
