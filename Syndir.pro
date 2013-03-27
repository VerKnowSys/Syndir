# Syndir - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#

QT                -= gui
QT                += network
HEADERS           += syndir.h \
                     file_watcher.h \
                     ssh_wrapper/Connection.h \
                     ssh_wrapper/Exception.h \
                     ssh_wrapper/UserInfo.h
SOURCES           += file_watcher.cc \
                     syndir.cc \
                     ssh_wrapper/Connection.cpp \
                     ssh_wrapper/Exception.cpp \
                     ssh_wrapper/UserInfo.cpp
TARGET            = syndir
INCLUDEPATH       += ${HOME}/Apps/Libssh2/include
LIBS              += ${HOME}/Apps/Libssh2/lib/libssh2.dylib -lcrypto
QMAKE_CXXFLAGS    += -std=c++11
