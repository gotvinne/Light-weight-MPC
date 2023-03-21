#!/bin/bash

# Make folders
mkdir -p "build"
cd build

# Building Release
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release -DWEB=ON -DC_FUNCTIONS:STRING='_add'
make 
