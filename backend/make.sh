#!/bin/bash
# Compiling backend directly using local data folder
# flags: 
#     -T int MPC horizon
#     -s string scenario name
#     -n bool new simulation
# 
# ex: sh make.sh -T 180 -s SingleWell -n

mkdir -p "build" #NB! Sometimes the old build folder needs to be deleted.

# CLI parser
argN=false
while getopts "ns:T:h" flag; 
do
  case ${flag} in
    T)
      argT=${OPTARG};;

    s) 
      argS=${OPTARG};;

    n) 
      argN=true;;

    ?|h)
      echo "Usage: $(basename $0) [-T int] [-s string] [-n]"
      exit 1;;
  esac
done

cd build
# Building Release
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX
make

# $? denote exit code
if [ $? -eq 0 ]; then #Build successful
    echo 

    if $argN; then # If flag is found
      ./backend -T $argT -s $argS -n
    else 
      ./backend -T $argT -s $argS
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
