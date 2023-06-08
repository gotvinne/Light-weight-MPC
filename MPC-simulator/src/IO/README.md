
## Input-Output, IO formatting
This module handles the inputs and outputs of the MPC-core, in fact the json data files. 

### Input format
- A json file defining **the system** (states, inputs, step coefficients and references)
- A **scenario file** defining the MPC applied to the corresponding system file. 

The [nlohmann/json](https://json.nlohmann.me/api/basic_json/) library is used in order to parse and formate the data. 
  
*System file (sr - step response / ss - state space)*:
```json
{
   "model": {
      "n_CV": int,
      "n_MV": int,
      "N": int, (#Largest Step-coefficient series)
   },

   "CV": [
      { 
         "output": "output_name",
         "init": double,
         "unit": string,
         "S": [[S11, S12, S13, ... , S1N],
               [S21, S22, S23, ... , S2N], 
               ... , 
               [S n_MV, ... , S n_MV N]]
      }, 
         ... ,
      { 
         "output": "output_name",
         "init": double,
         "unit": string,
         "S": [[S11, S12, S13, ... , S1N],
               [S21, S22, S23, ... , S2N], 
               ... , 
               [S n_MV, ... , S n_MV N]]
      }
   ],
   
   "MV": [
      {
         "input": "input_name", 
         "init": double,
         "unit": string
      },
         ... , 
      {
         "input": "input_name",
         "init": double,
         "unit": string
      } 
   ],                         
}
```
NB! Not every step coefficient series need to be N elements long. The software pads smaller vectors using $$S(N+1) \approx S(N)$$. 
 
*Scenario file*:
```json  
{
 "system": "system_name", 
 
 "MPC": {
   "P": int, (Prediction horizon)
   "M": int, (Control horizon)
   "W": int, (Time delay)
   "Q": [Q1, Q2, ... , Qn_CV], (Positive definite)
   "R": [R1, R2, ... , Rn_MV], (Positive definite)
   "RoH": [Ro1, Ro2, ..., Ro n_CV], (Upper slack variable)
   "RoL": [Ro1, Ro2, ..., Ro n_CV] (Lower slack variable)
 },

 "c": [ 
   {"du[1]": [low, high]}, (double)
   ...,
   {"du[n_MV]": [low, high]}, (double)
   {"u[1]": [low, high]}, (double)
   ..., 
   {"u[n_MV]": [low, high]}, (double)
   {"y[1]": [low, high]}, (double)
   ..., 
   {"y[n_CV]": [low, high]}, (double)
]
}
``` 

- Simulation without slack variables: Define empty slack variables
```json
"RoL": [] 
"RoH": []
```

NB! The indicator for the constraints is only used for readability and is not parsed directly by the software. Hence, as long as the constraints are lined up in the format [dU, u, y], the simulation will be correct. 

### Output format

- **The simulation file** contains every variable needed in order to assess the performance of the MPC configuration by plotting the data. 

*Simulation file*
```json  
{
 "scenario": "scenario_name", 
 "T": int,
 "n_CV": int,
 "n_MV": int, 
 "P": int, 
 "M": int,
 "du_tilde" : [[du11, du12, du13, ... , du1(N-1)],
         [du21, du22, du23, ... , du2(N-1)], 
         ... , 
         [du n_MV, ... , du n_MV (N-1)]],

 "CV": [ 
      {  
         "output": "output_name",
         "unit": string, 
         "c": [low, high] (double),
         "y": [y1, y2, y3, ... , y(T+P+1)], (Plant model simulation)
         "y_pred": [y1, y2, y3, ... , y(T+P+1)], (Predicted model simulation)
         "ref": [r1, r2, ..., r(T+P+1)] (reference data)
      }, 
         ... , 
      { 
         "output": "output_name",
         "unit": string, 
         "c": [low, high] (double),
         "y": [y1, y2, y3, ... , y(T+P+1)], (Plant model simulation)
         "y_pred": [y1, y2, y3, ... , y(T+P+1)], (Predicted model simulation)
         "ref": [r1, r2, ..., r(T+P+1)] (reference data)
      }
   ],

 "MV": [ 
      {
         "input": "input_name",
         "unit": string, 
         "c": [low, high], (double, constraining u)
         "u": [u1, u2, u3, ... , u(T+M)] 
      }, 
         ... , 
      { 
         "input": "input_name",
         "unit": string, 
         "c": [low, high], (double, constraining u)
         "u": [u1, u2, u3, ... , u(T+M)] 
      }
   ]
}
``` 

### Open loop simulation format:
When simulating an open loop process some simulation data is left out 

*Simulation file*
```json  
{
 "scenario": "scenario_name", 
 "T": int,
 "n_CV": int,
 "n_MV": int, 
 "P": int, 
 "M": int,
 "du_tilde" : [[du11, du12, du13, ... , du1(N-1)],
         [du21, du22, du23, ... , du2(N-1)], 
         ... , 
         [du n_MV, ... , du n_MV (N-1)]],

 "CV": [ 
      {  
         "output": "output_name",
         "unit": string, 
         "y_pred": [y1, y2, y3, ... , yT], (Predicted model simulation)
      }, 
         ... , 
      { 
         "output": "output_name",
         "unit": string, 
         "y_pred": [y1, y2, y3, ... , yT], (Predicted model simulation)
      }
   ],

 "MV": [ 
      {
         "input": "input_name",
         "unit": string, 
         "u": [u1, u2, u3, ... , uT] 
      }, 
         ... , 
      { 
         "input": "input_name",
         "unit": string, 
         "u": [u1, u2, u3, ... , uT] 
      }
   ]
}
``` 