# MPC-core: C++, Python, Webassembly
This is a simulation software base for automating MPC simulations on Finite Step Response Models. All data files, including the model description and controller definition, are defined using JSON format making the software easily integratable. Having defined the input files, one can simulate the controller from the command line, calling make.sh using CMake and GNU compiler. In order to visualize the simulation data, plotting functionality is implemented in Python. Additionally, in order to interface the software to web-applications, the software is compiled to wasm format calling emcc.sh using the Emscripten compiler. 

## Modules
- [data](data/README.md): System, scenario and simulation files.
- [MPC](src/MPC/README.md): Solving the MPC problems.
- [model](src/model/README.md): Generating a Finite Step Response Model. 
- [IO](src/IO/README.md): Parsing and serializing respectively input and output data.
- [vis](vis/README.md): Visualizing simulations in jupyter notebook

OsqpEigen is a folder holding the [osqp-eigen](https://github.com/robotology/osqp-eigen) software. 

### Dependancies:
This software is developed using a environment and package manager [conda](https://docs.conda.io/en/latest/#), builded using [CMake](https://cmake.org/) and compiled using [GCC](https://gcc.gnu.org/).

Other C/C++ compilers needed:
*Linux*: 
```console
sudo apt-get install g++
sudo apt-get install emscripten //For Webassembly
```

Other libraries used: 
- [OSQP](https://osqp.org/), Operator Splitting Quadratic program
- [osqp-eigen](https://github.com/robotology/osqp-eigen), C++ wrapper for OSQP 
- [nlohmann/json](https://json.nlohmann.me/api/basic_json/), Json parser
- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page), Template library for linear algebra
- [boost/odeint](http://headmyshoulder.github.io/odeint-v2/), Numerical ODE solver
- [CLI11](https://github.com/CLIUtils/CLI11), Command line parser
- [Emscripten](https://emscripten.org/docs/index.html), web compiler

### Run MPC-core: 
*From project root:* 

- Use conda in order to create environment access the environments in conda folder:
```console
conda env create -f env.yml
```

or make a new environment and install conda packages: 
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
- Build and run program, NB! make.sh calls binary ./mpc_core
Arguments:
- [-T int] mpc_horizon: Simulate scenario for the given mpc_horizon
- [-s string] scenario_name: Scenario file to be simulated
- [-r string] reference vector
- [-n bool] new simulation
```console
chmod +x setup.sh                       // Set execute permission
sh make.sh -T mpc_horizon -s sce -r ref -n
```
### Webassembly
Light-weight MPC uses the *Emscripten* compiler in order to compile the C++-code to *Webassembly* which can be reached from a website using JavaScript.
In order to interface web, binding files are requred the implementation of the interface functionality can be found in the *wasm* folder. 

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

Before building and running the compiler, the enscripten compiler needs to be sourced to terminal. Afterwards, emcc.sh can be run. The output, *webassembly.mjs* is stored in the Front-End's src-folder. emcc.sh takes an argument [-d dest] with is the filepath to the webassembly file
```console
source ./emsdk_env.sh
sh emcc.sh -d ../frontend/src
```

### Visualization
MPC-core has an secondary code base providing plot functionality. This software is developed in Python, and can be installed using [conda](https://docs.conda.io/en/latest/#). The visualization tool is found in the folder named */vis*

```console
conda install -n env -c anaconda jupyter
conda install -n env -c anaconda numpy
conda install -n env -c conda-forge matplotlib
```

One can choose to either open Jupyter-Notebook and run *vis.ipynb*. Alternatively, can the plots be produced in by a terminal command from root (Linux operating system):

```console
python3 vis/plot.py -s "Simulation"
```
