# Syndir - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#

SYSTEM_NAME        = $$system(uname)
! mac {
  CONFIG          += link_pkgconfig
  PKGCONFIG        = QtCore
}
QMAKE_CXX          = clang++
#DEFINES           += WITH_SFTP HAVE_POLL HAVE_SELECT HAVE_LIBCRYPTO HAVE_PTHREAD WITH_ZLIB HAVE_GETADDRINFO
QT                -= gui

HEADERS           += file_watchers_manager.h \
                     worker_thread.h \
                     syndir.h

SOURCES           += worker_thread.cc \
                     file_watchers_manager.cc \
                     syndir.cc


TARGET             = syndir

LIBS            += -lz -lcrypto ./libssh.a

contains(SYSTEM_NAME, Linux): {
  QMAKE_CXXFLAGS  += -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -DDEBUG=true
} else {
  QMAKE_CXXFLAGS  += -std=c++11 -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE
  # -DDEBUG=true
}
# QMAKE_LDFLAGS     +=
