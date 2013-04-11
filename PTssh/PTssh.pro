QMAKE_CXX = clang++
QMAKE_CC = clang

QMAKE_CXXFLAGS += -w

DEFINES += SHOW_STATISTICS PTSSH_SFTP _DEBUG

QT -= gui
TEMPLATE = lib
CONFIG += staticlib

HEADERS += ./*.h
SOURCES += ./*.cpp

TARGET = ../PTssh