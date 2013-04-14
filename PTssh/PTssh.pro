# PTssh SSH implementation - © 2013 verknowsys.com
#
# author:
#   Paul Thomas
#


include(../Common.pro)


DEFINES += SHOW_STATISTICS PTSSH_SFTP _DEBUG PTSSH_SFTP
QMAKE_CXXFLAGS += -w

TEMPLATE = lib
CONFIG += staticlib

HEADERS += *.h
SOURCES += *.cpp

TARGET = ../PTssh