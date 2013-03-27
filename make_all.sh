#!/bin/sh


readonly PROJECT_NAME="Syndir.pro"

if [ ! -x "/usr/bin/sofin" ]; then
    printf "VerKnowSys Sofin is required to build this software.\n"
    exit 1
fi

if [ ! -d "/Software/Qt/include" ]; then
    if [ ! -d "${HOME}/Apps/Qt" ]; then
        sofin install qt
    fi
fi

if [ ! -d "/Software/Bsdmake" ]; then
    if [ ! -d "${HOME}/Apps/Bsdmake" ]; then
        sofin install bsdmake
    fi
fi

# please note that this dependency is by default on user side:
if [ ! -d "${HOME}/Apps/Libssh2" ]; then
    sofin install libssh2
fi


qmake -spec darwin-g++ "${PROJECT_NAME}"
bsdmake CC=clang CXX=clang++
