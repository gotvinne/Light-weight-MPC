#!/bin/bash
# Compiling backend to webassembly

mkdir -p "build_emcc"
cd build_emcc

# Setting emsdk path
echo 'source "/home/gotvinne/emsdk/emsdk_env.sh"' >> $HOME/.bash_profile

# Building Release
emcmake cmake .. -DEMCC_FORCE_STDLIBS=1 -DWEBASSEMBLY=ON
emmake make 

# Update modified js file to ignore eslint errors
sed -i '1i/* eslint-disable */' ../../frontend/src/webassembly.mjs