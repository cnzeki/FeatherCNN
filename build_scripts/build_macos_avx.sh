#!/bin/bash

mkdir -p build-macos-avx
pushd build-macos-avx
cmake -DCMAKE_TOOLCHAIN_FILE=../build_scripts/macos.toolchain.cmake .. -DFEATHER_AVX=1 -DBUILD_HOST_APPLE=1
make -j4
make install
popd
