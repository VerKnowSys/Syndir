#!/bin/sh

if [ ! -x "/usr/bin/sofin" ]; then
    printf "Using Sofin for software installation is a preffered method for installing dependencies of Syndir. I don't support that case. Use on your own risk.\n"
    # exit 1
fi

if [ "$(uname)" != "Darwin" ]; then
    if [ ! -d "/Software/Qt/include" ]; then
        if [ ! -d "${HOME}/Apps/Qt" ]; then
            sofin install qt
        fi
    fi
fi

MAKE="make"
SPEC_TYPE=""
if [ "$(uname)" = "Darwin" ]; then
    SPEC_TYPE="-spec macx-llvm" #darwin-g++"
fi

if [ "${PACKAGE}" = "YES" ]; then
    printf "Preparing package. Performing clean build\n"
    make distclean
fi
qmake ${SPEC_TYPE} Project.pro
${MAKE} CC=clang CXX=clang++

if [ "${PACKAGE}" = "YES" ]; then
    printf "Creating software bundle\n"
    macdeployqt Synshot.app -dmg
    macdeployqt Syndir.app -dmg
    printf "Doing merge into Synshot"
    cp Syndir.app/Contents/MacOS/Syndir Synshot.app/Contents/MacOS/
    rm -rf ./Syndir.app
    printf "Creating archive\n"
    rm -f ./Synshot-${VERSION}.tar.gz
    VERSION="$(grep 'APP_VERSION' Syndir/syndir.h | awk '{ gsub(/\"/, "", $3);  print $3; }')"
    tar cjf ./Synshot-${VERSION}.tar.gz ./Synshot.app
fi

printf "\n"
exit 0



