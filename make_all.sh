#!/bin/sh


readonly PROJECT_NAME="Syndir.pro"

if [ ! -x "/usr/bin/sofin" ]; then
    printf "VerKnowSys Sofin is required to build this software.\n"
    exit 1
fi

sofin update
if [ ! -d "/Software/Qt/include" ]; then
    if [ ! -d "${HOME}/Apps/Qt" ]; then
        sofin install qt
    fi
fi

MAKE="bsdmake"
if [ "$(uname)" != "FreeBSD" ]; then
    if [ ! -d "/Software/Bsdmake" ]; then
        if [ ! -d "${HOME}/Apps/Bsdmake" ]; then
            sofin install bsdmake
        fi
    fi
else
    MAKE="make" # the only system with bsd make on board
fi

# please note that this dependency is by default on user side:
if [ ! -d "${HOME}/Apps/Libssh2" ]; then
    sofin install libssh2
fi


qmake -spec darwin-g++ "${PROJECT_NAME}"
${MAKE} CC=clang CXX=clang++
