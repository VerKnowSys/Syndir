# Syndir - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#

QT                -= gui
QT                += network

HEADERS           += syndir.h \
                     file_watchers_manager.h \
                     ssh_wrapper/Connection.h \
                     ssh_wrapper/Exception.h \
                     ssh_wrapper/UserInfo.h

SOURCES           += syndir.cc \
                     ssh_wrapper/Connection.cpp \
                     ssh_wrapper/Exception.cpp \
                     ssh_wrapper/UserInfo.cpp \
                     file_watchers_manager.cc

TARGET            = syndir

INCLUDEPATH       += ${HOME}/Apps/Libssh2/include
LIBS              += ${HOME}/Apps/Libssh2/lib/libssh2.${LIBTYPE} -lcrypto
QMAKE_CXXFLAGS    += -std=c++11 -Wl,-rpath=${HOME}/Apps/Libssh2/lib,--enable-new-dtags -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -DDEBUG=true
QMAKE_LDFLAGS     += -Wl,-rpath=${HOME}/Apps/Libssh2/lib,--enable-new-dtags
