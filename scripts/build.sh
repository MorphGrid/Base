#!/usr/bin/env bash
set -euo pipefail

BUILD_TYPE=${1:-debug}
LINK_TYPE=${2:-static}

mkdir -p build
cd build

if [[ "$LINK_TYPE" == "static" ]]; then
ENABLE_STATIC="ON"
else
ENABLE_STATIC="OFF"
fi

if [[ "$BUILD_TYPE" == "debug" ]]; then
DEBUG_ENABLED="ON"
else
DEBUG_ENABLED="OFF"
fi
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DBUILD_PROGRAM_TESTS=OFF -DENABLE_CI=ON -DDEBUG_ENABLED=$DEBUG_ENABLED -DENABLE_STATIC_LINKING=$ENABLE_STATIC

make -j4