#!/bin/bash
# Check if the folder exists
if [[ ! -d "/build" ]]
then
    mkdir build
fi

cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX
make

# $? denote exit code
if [ $? -eq 0 ]; then #Build successful
    echo 
    ./light_weight
    if [ $? -eq 0 ]; then #Running successfull
        echo
    else
        RED='\033[0;31m'
        NC='\033[0m' 
        echo "${RED}RUNTIME ERROR! ${NC}\n"
    fi
else
    RED='\033[0;31m'
    NC='\033[0m' 
    echo "${RED}BUILD ERROR! ${NC}\n"
fi
