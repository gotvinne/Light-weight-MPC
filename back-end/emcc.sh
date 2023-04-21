#!/bin/bash
# Compiling backend to webassembly

mkdir -p "build_emcc"
cd build_emcc

# Setting emsdk path
echo 'source "/home/gotvinne/emsdk/emsdk_env.sh"' >> $HOME/.bash_profile

# Building Release
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release -DWEBASSEMBLY=ON
emmake make 
