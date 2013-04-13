# Syndir + Synshot - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#

SYSTEM_NAME        = $$system(uname)
! mac {
  CONFIG          += link_pkgconfig
  PKGCONFIG        = QtCore
}
DEFINES           += PTSSH_SFTP SHOW_STATISTICS _DEBUG
QMAKE_CXX          = clang++
QMAKE_CC           = clang
# QT                -= gui

TEMPLATE           = subdirs
SUBDIRS            = PTssh Syndir Synshot

# HEADERS           += Syndir/file_watchers_manager.h \
#                      Syndir/worker_thread.h \
#                      Syndir/syndir.h

# SOURCES           += Syndir/worker_thread.cc \
#                      Syndir/file_watchers_manager.cc \
#                      Syndir/syndir.cc
#

# TARGET             = syndir

# mac {
#   INCLUDEPATH     += PTssh/
#   LIBS            += -lz -lcrypto libPTssh.a
# } else {
#   LIBS            += -lz -lcrypto libPTssh.a
# }
#
# contains(SYSTEM_NAME, Linux): {
#   QMAKE_CXXFLAGS  += -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -DDEBUG=true
# } else {
#   QMAKE_CXXFLAGS  += -std=c++11 -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -w
#   # -DDEBUG=true
# }
# # QMAKE_LDFLAGS     +=
