#!/bin/bash
mkdir -p "build"

cd build
# Building Release
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX
make

# $? denote exit code
if [ $? -eq 0 ]; then #Build successful
    echo 

    if $argN; then # If flag is found
      ./light_weight
    else 
      ./light_weight
    fi

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
