# Project thesis: Light-weight-MPC

This is a repo for implementing the project thesis for the study programme [Cybernetics & Robotics](https://www.ntnu.no/studier/mttk) at **NTNU** the fall 2022. The thesis is handed out by **Equinor**, and aims on implementing a simpler software framework simulating optimized control on step response models. 

[Project thesis description](https://www.itk.ntnu.no/ansatte/imsland_lars/projects2022.html)

The documentation is generated using [Doxygen](https://doxygen.nl/)

Operating system: Linux

#### Master student: 
- Geir Ola Tvinnereim gotvinne@stud.ntnu.no

#### Supervisors:
- Prof. Lars Struen Imsland (ITK) lars.imsland@ntnu.no
- Prof. Gisle Otto Eikrem (Equinor) gise@equinor.com

### Modules
- [OSQP-Eigen](src/OSQP-Eigen/README.md): Solving the MPC problems. 
- [model](src/model/README.md): Generating a FSRM model.
- [data](data/README.md): System and scenario files defining the simulation.
- [IO](src/IO/README.md): Parsing and formatting respectively input and output data.
- [Vis](vis/README.md): Visualizing simulations in jupyter notebook

### Dependancies:
This software is developed using a environment and package manager [Anaconda](https://www.anaconda.com/products/distribution) and builded using [CMake](https://cmake.org/)

Other libraries used: 
- [OSQP](https://osqp.org/), Operator Splitting Quadratic program (Source code)
- [osqp-eigen](https://github.com/robotology/osqp-eigen), C++ wrapper for OSQP 
- [nlohmann/json](https://json.nlohmann.me/api/basic_json/), Json parser
- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page), Template library for linear algebra
- [boost/odeint](http://headmyshoulder.github.io/odeint-v2/), Numerical ODE solver
- [CLI11](https://github.com/CLIUtils/CLI11), Command line parser

### Run Light-weight-MPC: 
*From project root:* 

- Use Anaconda in order to create environment:
```console
conda env create -f env.yml
```

or make a new environment, *env*, and install conda packages: 
```console
conda install -n env -c anaconda cmake
conda install -n env -c conda-forge osqp-eigen
conda install -n env -c conda-forge nlohmann_json
conda install -n env -c conda-forge boost
conda install -n env -c anaconda jupyter
```

- Build and run program
```console
sh setup.sh
```

### LICENCE:
Licence of osqp-eigen
