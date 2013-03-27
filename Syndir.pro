# Syndir - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#

QT                -= gui
QT                += network
HEADERS           += syndir.h file_watcher.h
SOURCES           += file_watcher.cc syndir.cc
TARGET            = syndir
INCLUDEPATH       += ${HOME}/Apps/Libssh2/include
QMAKE_LIBDIR      += ${HOME}/Apps/Libssh2/lib
QMAKE_CXXFLAGS    += -std=c++11
