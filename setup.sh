#!/bin/bash 

cd build

conan install ..
cmake ..
make
./light_weight