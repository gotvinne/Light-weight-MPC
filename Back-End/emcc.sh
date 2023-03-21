#!/bin/bash

mkdir -p "build_emcc"
cd build_emcc

# Building Release
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release -DWEB=ON -DC_FUNCTIONS:STRING='_add'
make 
