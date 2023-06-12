#!/bin/bash
# Compiling backend to webassembly
# flags: 
#     -d mjs file destination
# 
# ex: sh emcc.sh -d ../Web-application/src

# NB! For linux and windows the compiler needs to be sourced!
mkdir -p "build_emcc"
cd build_emcc

# Setting emsdk path, REMOVE THIS LINE!
echo 'source "/home/gotvinne/emsdk/emsdk_env.sh"' >> $HOME/.bash_profile

while getopts "d:h" flag; 
do
  case ${flag} in
    d)
      dest=${OPTARG};;

    ?|h)
      echo "WRONG CLI, Usage: $(basename $0) [-d string]"
      exit 1;;
  esac
done

# Building Release
emcmake cmake .. -DEMCC_FORCE_STDLIBS=1 -DWEBASSEMBLY=ON -DDEST=$dest
emmake make 

# Update modified js file to ignore eslint errors
sed -i '1i/* eslint-disable */' ../$dest/mpc_simulator.mjs