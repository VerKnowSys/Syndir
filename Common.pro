# Syndir Common - © 2013 verknowsys.com
#
# author:
#   Daniel (dmilith) Dettlaff
#


QT -= gui

QMAKE_CXX = ccache clang++
QMAKE_CC = ccache clang

QMAKE_CXXFLAGS += -fcolor-diagnostics -Qunused-arguments -Wself-assign -fPIC -fPIE -Os -w

mac {

  QMAKE_CXXFLAGS  += -std=c++11

} else {

  CONFIG += link_pkgconfig
  PKGCONFIG = QtCore

}

