#!/bin/bash

mkdir -p "build_emcc"
cd build_emcc

functions="'_add','_validSystem'" # "_func1,_func2,..."

# Building Release
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release -DWEB=ON -DC_FUNCTIONS:STRING=$functions
make 
