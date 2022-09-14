# Project thesis: Light-weight-MPC

This is a repo for implementing the project thesis for the study programme [Cybernetics & Robotics](https://www.ntnu.no/studier/mttk) at **NTNU** the fall 2022. The thesis is handed out by **Equinor**, and aims on implementing a simpler software framework simulating optimized control on step response models. 

[Project thesis description](https://www.itk.ntnu.no/ansatte/imsland_lars/projects2022.html)

The documentation is generated using [Doxygen](https://doxygen.nl/)
Operating system: Linux

#### Master student: 
- Geir Ola Tvinnereim gotvinne@stud.ntnu.no

#### Supervisors:
- Lars Struen Imsland (ITK) lars.imsland@ntnu.no
- Gisle Otto Eikrem (Equinor) gise@equinor.com

##### Input format
Step response MIMO model without inequality constraints:
```json  
{
 "system": "modelname", (sr - step response / ss - state space)
 "T": int, (Simulation steps)
 
 "model": {
   "n_CV": int,
   "n_MV": int,
   "N": int, (#Step-response coefficients)
 },

 "CV": { 
   [
      { 
         "description": "state", (Name of state)
         "id": int,
         "init": double,
         "S": [[S11, S12, S13, ... , S1N],
               [S21, S22, S23, ... , S2N], 
               ... , 
               [S n_MV, ... , S n_MV N]] 
      }, 
         ... ,
      { 
         "description": "state", (Name of state)
         "id": int,
         "init": double,
         "S": [[S11, S12, S13, ... , S1N],
               [S21, S22, S23, ... , S2N], 
               ... , 
               [S n_MV, ... , S n_MV N]] 
      }
   ]
 }, 

 "MV": { 
   [
      {
         "description": "input", (Name of input)
         "id": int, 
         "init": double 
      },
         ... , 
      {
         "description": "input", (Name of input)
         "id": int, 
         "init": double 
      } 
   ], 
 },
 
 "mpc": {
   "P": int, (Prediction horizon)
   "M": int, (Control horizon)
   "Q": [Q1, Q2, ... , QP],
   "R": [R1, R2, ... , RP],
   "ρ": int (Slack variable)
 }
}
``` 
[Json parser C++](https://linuxhint.com/parse-json-data-cpp/)

##### Output format
```json  
{
 "CV": 
   [ 
      {
         "description": "state",
         "sim": [x1, x2, x3, ... , xT] 
      }, 
         ... , 
      { 
         "description": "state",
         "sim": [x1, x2, x3, ... , xT] 
      }
   ]
 "MV": 
   [ 
      {
         "description": "input",
         "sim": [u1, u2, u3, ... , uT] 
      }, 
         ... , 
      { 
         "description": "input",
         "sim": [u1, u2, u3, ... , uT] 
      }
   ]
}
``` 

### Dependancies:
This software is developed using a package manager [Conan](https://conan.io/) and builded using [CMake](https://cmake.org/)

Installing Conan
```console
sudo pip install conan
```

- OSQP, Operator Splitting Quadratic program [https://osqp.org/]
- [nlohmann/json](https://github.com/nlohmann/json)


### Run:
```console
sh setup.sh
```





