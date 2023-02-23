#!/bin/bash

set -ex

BUILD_TYPE=Release

# Conan
apt-get update
apt-get install -y python3-pip
pip3 install conan
conan profile detect

if [[ "${CXX}" == clang* ]]
then
    export CXXFLAGS="-stdlib=libc++"
    sed -i 's/^compiler.libcxx=.*$/compiler.libcxx=libc++/g' ~/.conan2/profiles/default
fi

mkdir build && cd build

conan install --build=missing ..

cmake -DCMAKE_TOOLCHAIN_FILE=./conan_toolchain.cmake -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" ..
make
make unittest
