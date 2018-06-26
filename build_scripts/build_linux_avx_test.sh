#!/bin/bash

./build_scripts/build_linux_avx.sh
g++ ./test/test.cpp -I./build-linux-avx/install/feather/include/ -L ./build-linux-avx/install/feather/lib/ -lfeather -fopenmp -O3 -o feather_benchmark
