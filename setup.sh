#!/bin/bash 
cd build

conan install ..
cmake ..
make

if [ $? -eq 0 ]; then
    echo 
    ./bin/light_weight
else
    RED='\033[0;31m'
    NC='\033[0m' 
    echo "${RED}BUILD ERROR! ${NC}\n"
fi
