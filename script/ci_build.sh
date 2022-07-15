#!/bin/bash

set -ex

# Conan
apt-get update
apt-get install -y python3-pip
pip3 install conan
conan profile new default --detect

if [[ "${CXX}" == clang* ]]
then
    export CXXFLAGS="-stdlib=libc++"
    conan profile update settings.compiler.libcxx=libc++ default
else
    conan profile update settings.compiler.libcxx=libstdc++11 default
fi

conan profile update settings.compiler.cppstd=17 default

mkdir build && cd build

conan install --build=missing ..

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake ..
make
make unittest
