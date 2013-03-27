# Syndir - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#

QT                -= gui
QT                += network
HEADERS           += ssh_wrapper/Connection.h \
                     ssh_wrapper/Exception.h \
                     ssh_wrapper/UserInfo.h \
                     syndir.h \
                     file_watchers_manager.h
SOURCES           += ssh_wrapper/Connection.cpp \
                     ssh_wrapper/Exception.cpp \
                     ssh_wrapper/UserInfo.cpp \
                     syndir.cc \
                     file_watchers_manager.cc
TARGET            = syndir
INCLUDEPATH       += ${HOME}/Apps/Libssh2/include
LIBS              += ${HOME}/Apps/Libssh2/lib/libssh2.dylib -lcrypto
QMAKE_CXXFLAGS    += -std=c++11
