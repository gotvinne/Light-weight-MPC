# Back-end: C++, Python, Webassembly

## Modules

- [data](data/README.md): System and scenario files defining the simulation.
- [MPC](src/MPC/README.md): Solving the MPC problems. 
- [model](src/model/README.md): Generating a FSRM model.
- [IO](src/IO/README.md): Parsing and formatting respectively input and output data.
- [Vis](vis/README.md): Visualizing simulations in jupyter notebook

### Dependancies:
This software is developed using a environment and package manager [conda](https://docs.conda.io/en/latest/#), builded using [CMake](https://cmake.org/) and compiled using [GCC](https://gcc.gnu.org/).

Other C/C++ compilers needed:
*Linux*: 
```console
sudo apt-get install g++
sudo apt-get install emscripten
```

Other libraries used: 
- [OSQP](https://osqp.org/), Operator Splitting Quadratic program (Source code)
- [osqp-eigen](https://github.com/robotology/osqp-eigen), C++ wrapper for OSQP 
- [nlohmann/json](https://json.nlohmann.me/api/basic_json/), Json parser
- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page), Template library for linear algebra
- [boost/odeint](http://headmyshoulder.github.io/odeint-v2/), Numerical ODE solver
- [CLI11](https://github.com/CLIUtils/CLI11), Command line parser
- [Emscripten](https://emscripten.org/docs/index.html), web compiler

### Run Light-weight MPC: 
*From project root:* 

- Use conda in order to create environment access the environments in conda folder:
```console
conda env create -f env.yml
```

or make a new environment, *light_weight*, and install conda packages: 
```console
conda create --name new_environment
```
```console
conda install -n env -c anaconda cmake
conda install -n env -c conda-forge osqp-eigen
conda install -n env -c conda-forge nlohmann_json
conda install -n env -c conda-forge cli11
conda install -n env -c conda-forge boost
```
```console
conda env export > conda/env.yml
```
- Build and run program, NB! make.sh calls binary ./light_weight
Arguments:
- [-T int] mpc_horizon: Simulate scenario for the given mpc_horizon
- [-s string] scenario_name: Scenario file to be simulated
- [-n bool] new simulation
```console
chmod +x setup.sh                       // Set execute permission
sh make.sh -T mpc_horizon -s sce -n
```
### Webassembly
Light-weight MPC uses the *Emscripten* compiler in order to compile the C++-code to a *Webassembly* which can be reached from a website using JavaScript.

In order to interface web, binding files are requred the implementation of the interface functionality can be found in the *bindings* folder. 

Install the Webassembly compiler locally, [online documentation](https://emscripten.org/docs/getting_started/downloads.html#platform-notes-installation-instructions-sdk): 
```console
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
./emsdk install latest
./emsdk activate latest
```
Or for MacOS, download via Brew
```console
brew install emscripten
```

Before building and running the compiler, the enscripten compiler needs to be sourced to terminal. Afterwards, emcc.sh can be run. The output, *webassembly.mjs* is stored in the Front-End's src-folder.  
```console
source ./emsdk_env.sh
sh emcc.sh
```

### Plot
For the **Light-weight MPC** software there are build in additional tools for plotting the simulation data. This software is developed in Python, and can be installed using [conda](https://docs.conda.io/en/latest/#). The visualization tool is found in the folder named */vis*

```console
conda install -n env -c anaconda jupyter
conda install -n env -c anaconda numpy
conda install -n env -c conda-forge matplotlib
```

One can choose to either open Jupyter-Notebook and run *vis.ipynb*. Alternatively, can the plots be produced in by a terminal command from root:

```console
python3 vis/plot.py -s "Simulation"
```
