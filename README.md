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

##### Input format
- A json file defining the system (states, inputs, step coefficients and references)
- A scenario file defining the MPC applied to the corresponding system file. 

  
*System file (sr - step response / ss - state space)*:
```json
{
   "model": {
      "n_CV": int,
      "n_MV": int,
      "N": int, (#Step-response coefficients)
   },

   "CV": [
      { 
         "state": "state_name",
         "id": "y[1]",
         "init": float,
         "S": [[S11, S12, S13, ... , S1N],
               [S21, S22, S23, ... , S2N], 
               ... , 
               [S n_MV, ... , S n_MV N]],
      }, 
         ... ,
      { 
         "state": "state_name",
         "id": "y[n_CV]",
         "init": float,
         "S": [[S11, S12, S13, ... , S1N],
               [S21, S22, S23, ... , S2N], 
               ... , 
               [S n_MV, ... , S n_MV N]],
         
      }
   ],
   
   "MV": [
      {
         "input": "input_name", 
         "id": "u[1]", 
         "init": float,
         "u": [r1, r2, r3, ... , uT] (Setpoint trajectory)
      },
         ... , 
      {
         "input": "input_name",
         "id": "u[n_MV]", 
         "init": float,
         "u": [u1, u2, u3, ... , uT] (Setpoint trajectory)
      } 
   ],                         
}
```

*Scenario file*:
```json  
{
 "system": "system_name", (sr - step response / ss - state space)
 
 "MPC": {
   "P": int, (Prediction horizon)
   "M": int, (Control horizon)
   "W": int, (Time delay)
   "Q": [Q1, Q2, ... , QP], (Positive definite - diagonal matrix with positive elements)
   "R": [R1, R2, ... , RP], (Positive definite)
   "Ro": float, (Slack variable)
   "bias update": bool
 },

 "c_i": [
   {"du[1]": [low, high]}, (float)
   ...,
   {"du[n_MV]": [low, high]}, (float)
   {"u[1]": [low, high]}, (float)
   ...,
   {"u[n_MV]": [low, high]}, (float)
   {"y[1]": [low, high]}, (float)
   ...,
   {"y[n_CV]": [low, high]} (float)
 ]
}
``` 

- Simulation without slack variables: 
```json
"Ro": null 
```
- Simulation without integral effect:
```json
"bias update": false 
```

##### Output format
```json  
{
 "CV": [ 
      {  // This is only the predicted states, the simulation uses a model.
         "state": "state_name",
         "y_hat[1]": [y1, y2, y3, ... , yT] 
      }, 
         ... , 
      { 
         "state": "state_name",
         "y_hat[n_CV]": [y1, y2, y3, ... , yT] 
      }
   ],

 "MV": [ 
      {
         "input": "input_name",
         "u[1] ": [u1, u2, u3, ... , uT] 
      }, 
         ... , 
      { 
         "input": "input_name",
         "u[n_MV]": [u1, u2, u3, ... , uT] 
      }
   ]
}
``` 

### Dependancies:
This software is developed using a environment and package manager [Anaconda](https://www.anaconda.com/products/distribution) and builded using [CMake](https://cmake.org/)

```console 
conda install -c anaconda cmake
```

- [OSQP](https://osqp.org/), Operator Splitting Quadratic program (Source code)
- [osqp-eigen](https://github.com/robotology/osqp-eigen), C++ wrapper for OSQP 
- [nlohmann/json](https://json.nlohmann.me/api/basic_json/)


### Run project: 
From project root: 

- Use Anaconda in order to create environment:
```console
conda env create -f env.yml
```

or make a new environment and install conda packages: 
```console
conda install -n env -c conda-forge osqp-eigen
conda install -n env -c conda-forge nlohmann_json
```

- Build and run program
```console
sh setup.sh
```

### LICENCE:
Licence of osqp-eigen





