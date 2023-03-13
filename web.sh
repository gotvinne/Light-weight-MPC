#!/bin/bash
mkdir -p "build"
mkdir -p "public"
cd build

# Building Release
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release -DWEB=ON
make 
