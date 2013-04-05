#!/bin/sh


readonly PROJECT_NAME="Syndir.pro"

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
${MAKE} LIBTYPE=${LIBTYPE} CXX=clang++

if [ "$(uname)" = "Darwin" ]; then
    printf "Building synshot\n"
    cd screenshot_sync_app
    if [ "${PACKAGE}" = "YES" ]; then
        printf "Preparing package. Performing clean build\n"
        make distclean
    fi
    qmake -spec macx-llvm "Synshot.pro"
    ${MAKE} LIBTYPE=${LIBTYPE} CXX=clang++
    if [ "$?" != "0" ]; then
        cd ..
        exit 1
    fi
    cd ..
    if [ "${PACKAGE}" = "YES" ]; then
        printf "Creating software bundle\n"
        macdeployqt Synshot.app -dmg
        printf "Creating archive\n"
        tar cjf ./Synshot.tar.gz ./Synshot.app
    fi
fi

exit 0



