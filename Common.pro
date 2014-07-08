# Syndir Common - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#


QT -= gui

QMAKE_CXX = ccache clang++
QMAKE_CC = ccache clang

OBJECTS_DIR = ../.obj
MOC_DIR = ../.moc

QMAKE_CXXFLAGS += -fcolor-diagnostics -Qunused-arguments -fPIC -fPIE -O2 -w -DQT5_ENABLED -std=c++11

