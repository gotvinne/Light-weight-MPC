#!/bin/bash
# Compiling backend to webassembly

mkdir -p "build_emcc"
cd build_emcc

# Building Release
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release -DWEBASSEMBLY=ON
emmake make 
