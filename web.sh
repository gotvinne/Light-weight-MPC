#!/bin/bash

# Delete old files:
rm -rf build
rm -rf public

# Make folders
mkdir -p "build"
mkdir -p "public"
cd build

# Building Release
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release -DWEB=ON -DC_FUNCTIONS:STRING='_add'
make 
