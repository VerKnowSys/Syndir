#!/bin/sh


readonly PROJECT_NAME="Syndir.pro"

if [ ! -x "/usr/bin/sofin" ]; then
    printf "Using Sofin for software installation is a preffered method for installing dependencies of Syndir. I don't support that case. Use on your own risk.\n"
    # exit 1
fi

sofin update
if [ "$(uname)" != "Darwin" ]; then
    if [ ! -d "/Software/Qt/include" ]; then
        if [ ! -d "${HOME}/Apps/Qt" ]; then
            sofin install qt
        fi
    fi
fi

MAKE="make"
SPEC_TYPE=""
LIBTYPE="a"
if [ "$(uname)" = "Darwin" ]; then
    SPEC_TYPE="-spec darwin-g++"
    # LIBTYPE="dylib"
fi

# please note that this dependency is by default on user side:
if [ "$(id -u)" = "0" ]; then
    printf "Cannot be build by root for now.\n"
    exit 1
else
    if [ ! -d "${HOME}/Apps/Libssh2" ]; then
        sofin install libssh2
    fi
fi


qmake ${SPEC_TYPE} "${PROJECT_NAME}"
${MAKE} LIBTYPE=${LIBTYPE} CC=clang CXX=clang++
exit 0
