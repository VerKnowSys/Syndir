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
if [ "$(uname)" = "Darwin" ]; then
    SPEC_TYPE="-spec darwin-g++"
fi

# cd libssh
# qmake ${SPEC_TYPE} "ssh.pro"
# ${MAKE} CC=clang CXX=clang++
# cd ..

qmake ${SPEC_TYPE} "${PROJECT_NAME}"
${MAKE} CC=clang CXX=clang++

if [ "${NOGUI}" = "YES" ]; then
    exit
fi

if [ "$(uname)" = "Darwin" ]; then
    printf "Building synshot\n"
    cd screenshot_sync_app
    if [ "${PACKAGE}" = "YES" ]; then
        printf "Preparing package. Performing clean build\n"
        make distclean
    fi
    qmake -spec macx-llvm "Synshot.pro"
    ${MAKE} CC=clang CXX=clang++
    if [ "$?" != "0" ]; then
        cd ..
        exit 1
    fi
    cd ..
    if [ "${PACKAGE}" = "YES" ]; then
        printf "Creating software bundle\n"
        macdeployqt Synshot.app -dmg
        printf "Creating archive\n"
        rm -f ./Synshot-${VERSION}.tar.gz
        VERSION="$(grep 'APP_VERSION' syndir.h | awk '{ gsub(/\"/, "", $3);  print $3; }')"
        tar cjf ./Synshot-${VERSION}.tar.gz ./Synshot.app
    fi
fi

printf "\n"
exit 0



