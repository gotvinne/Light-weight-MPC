# Project thesis: Light-weight-MPC

This is a repo for implementing the project thesis for the study programme [Cybernetics & Robotics](https://www.ntnu.no/studier/mttk) at **NTNU** the fall 2022. The thesis is handed out by **Equinor**, and aims on implementing a simpler software framework simulating optimized control on step response models. 

[Project thesis description](https://www.itk.ntnu.no/ansatte/imsland_lars/projects2022.html)

The documentation is generated using [Doxygen](https://doxygen.nl/)

Operating system: Linux

#### Master student: 
- Geir Ola Tvinnereim gotvinne@stud.ntnu.no

#### Supervisors:
- Prof. Lars Struen Imsland (ITK) lars.imsland@ntnu.no
- Dr. Gisle Otto Eikrem (Equinor) gise@equinor.com

### Modules

- [data](data/README.md): System and scenario files defining the simulation.
  
- [OSQP-Eigen](src/OSQP-Eigen/README.md): Solving the MPC problems. 
- [model](src/model/README.md): Generating a FSRM model.
- [IO](src/IO/README.md): Parsing and formatting respectively input and output data.
- [Vis](vis/README.md): Visualizing simulations in jupyter notebook

### Dependancies:
This software is developed using a environment and package manager [conda](https://docs.conda.io/en/latest/#), builded using [CMake](https://cmake.org/) and compiled using [GCC](https://gcc.gnu.org/).

Other C/C++ compilers needed:
```console
sudo apt-get install g++
sudo apt install libexpat1-dev
sudo apt install libxml2-dev
```

Other libraries used: 
- [OSQP](https://osqp.org/), Operator Splitting Quadratic program (Source code)
- [osqp-eigen](https://github.com/robotology/osqp-eigen), C++ wrapper for OSQP 
- [nlohmann/json](https://json.nlohmann.me/api/basic_json/), Json parser
- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page), Template library for linear algebra
- [boost/odeint](http://headmyshoulder.github.io/odeint-v2/), Numerical ODE solver
- [CLI11](https://github.com/CLIUtils/CLI11), Command line parser


### Functional Mock-up Interface (FMI)
This is an industrial standard for simulating dynamical systems, while simultaneously perserve the confidensiality of the model. 

[FMI](https://fmi-standard.org/)

### Run Light-weight-MPC: 
*From project root:* 

- Use conda in order to create environment *light_weight*:
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

conda install -n env -c anaconda jupyter
conda install -n env -c anaconda numpy
conda install -n env -c conda-forge matplotlib
```
```console
conda env export > env.yml
```

- Build and run program, NB! setup.sh calls binary ./light_weight
Arguments:
- [-T int] mpc_horizon: Simulate scenario for the given mpc_horizon
- [-s string] scenario_name: Scenario file to be simulated
```console
chmod +x setup.sh                       // Set execute permission
sh setup.sh -T mpc_horizon -s sce
```

### LICENCE:
Licence of osqp-eigen
