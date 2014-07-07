#!/bin/bash
set -e  # exit immediately on error
set -v  # verbose, echo all commands

SOURCE_DIR=$(pwd)
BUILD_DIR=build-sqlitewrapper
CMAKE=cmake
MAKE=make -j2

export PATH="/usr/local/bin:$PATH"

cd ..
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
$CMAKE "$SOURCE_DIR"
$MAKE