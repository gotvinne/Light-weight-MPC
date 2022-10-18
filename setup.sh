#!/bin/bash
# Check if the folder exists
if [[ ! -d "/build" ]]
then
    mkdir build
fi

cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX
make

if [ $? -eq 0 ]; then
    echo 
    ./light_weight
    echo
else
    RED='\033[0;31m'
    NC='\033[0m' 
    echo "${RED}BUILD ERROR! ${NC}\n"
fi
