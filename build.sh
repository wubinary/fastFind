#!/bin/bash

# ================================================================================================ #
# Environ setup
# ================================================================================================ #
function usage_error() {
    echo
    echo "Usage:"
    echo " ./build.sh BUILD_TYPE<Release/Debug> "
    echo
    exit 1
}

# binary dir
BINARY_DIR=~/.local/bin

# build dir
BUILD_DIR=out/build

# build type (Debug/Release)
BUILD_TYPE=$1

# check
if [[ $BUILD_TYPE != "Release" ]] && [[ $BUILD_TYPE != "Debug" ]]; then
    usage_error
fi


# ================================================================================================ #
# Prepare build dir
# ================================================================================================ #
rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
mkdir -p ${BINARY_DIR}


# ================================================================================================ #
# Build
# ================================================================================================ #
cmake -S ./ -B ${BUILD_DIR} \
      -DBUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

(cd ${BUILD_DIR}; make)

ln -sf ${BUILD_DIR}/compile_commands.json compile_commands.json
ln -sf ${BUILD_DIR}/src/main_exec ff
rsync -L ff ${BINARY_DIR}/ff
