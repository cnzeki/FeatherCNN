#!/bin/bash

mkdir build-linux-avx
pushd build-linux-avx
cmake .. -DFEATHER_AVX=1
make -j4 VERBOSE=1
make install
popd
