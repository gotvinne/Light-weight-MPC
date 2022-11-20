#!/bin/bash

# CLI parser
while getopts ':T:h' opt; do
  case "$opt" in
    T)
      arg="$OPTARG"
      echo "Processing option 'c' with '${OPTARG}' argument"
      ;;

    ?|h)
      echo "Usage: $(basename $0) [-T int]"
      exit 1
      ;;
  esac
done
shift "$(($OPTIND -1))"

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
    ./light_weight -T $arg
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
