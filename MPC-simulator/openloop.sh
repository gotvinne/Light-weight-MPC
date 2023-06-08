#!/bin/bash
# Compiling backend directly using local data folder
# flags: 
#     -T int MPC horizon
#     -s string System name
#     -r std::vector<double> reference vector
#     -a std::vector<double> step vector
# 
# NB! refernce and step vector can only take on integer values
# ex: sh openloop.sh -T 180 -s SingleWell -r [3800,70] -a [5,25]

mkdir -p "build" #NB! Sometimes the old build folder needs to be deleted.

while getopts "s:r:a:T:h" flag; 
do
  case ${flag} in
    T)
      argT=${OPTARG};;
    s) 
      argS=${OPTARG};;
    r) 
      argRef=${OPTARG};;
    a) 
      argStep=${OPTARG};;

    ?|h)
      echo "WRONG CLI, Usage: $(basename $0) [-T int] [-s string] [-r std::vector<double>] [-a std::vector<double>]"
      exit 1;;
  esac
done

cd build
# Building Release
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX
make

RED='\033[0;31m' # Enable color
NC='\033[0m' 

# $? denote exit code
if [ $? -eq 0 ]; then #Build successful
    echo 

    # Calling open loop simulation -o
    ./mpc_simulator -T $argT -s $argS -r $argRef -a $argStep -o

    if [ $? -eq 0 ]; then #Running successfull
        echo
    else
       echo "${RED}RUNTIME ERROR! ${NC}\n"
    fi
else
    echo "${RED}BUILD ERROR! ${NC}\n"
fi
