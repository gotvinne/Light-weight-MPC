
## Input-Output, IO formatting

This module handles the inputs and outputs of the Light-Weight software, in fact the json data files. 

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
      "N": int, (#Step-response coefficients)
   },

   "CV": [
      { 
         "output": "output_name",
         "init": double,
         "unit": string,
         "y_ref": [y1, y2, y3, ... , yT], (Setpoint trajectory)
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
         "y_ref": [y1, y2, y3, ... , yT], (Setpoint trajectory)
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

*Scenario file*:
```json  
{
 "system": "system_name", (sr - step response / ss - state space)
 
 "MPC": {
   "P": int, (Prediction horizon)
   "M": int, (Control horizon)
   "W": int, (Time delay)
   "Q": [Q1, Q2, ... , QP], (Positive definite)
   "R": [R1, R2, ... , RM], (Positive definite)
   "Ro": double, (Slack variable)
   "bias update": bool
 },

 "c": [ // Consider only take inn one constraint on du, u, y
   {"du[1]": [low, high]}, (double)
   ...,
   {"du[M * n_MV]": [low, high]}, (double)
   {"u[1]": [low, high]}, (double)
   ...,
   {"u[M * n_MV]": [low, high]}, (double)
   {"y[1]": [low, high]}, (double)
   ...,
   {"y[P * n_CV]": [low, high]} (double)
 ]
}
``` 

- Simulation without slack variables: 
```json
"Ro": 0
```
- Simulation without integral effect:
```json
"bias update": false 
```

### Output format

- **The simulation file** contains every variable needed in order to assess the performance of the MPC configuration by plotting the data. 

*Simulation file*
```json  
{
 "scenario": "scenario_name", 
 "T": int,
 "n_CV": int,
 "n_MV": int, 
 
 "CV": [ 
      {  // This is only the predicted states, the simulation uses a model.
         "output": "output_name",
         "unit": string, 
         "c": [low, high] (double),
         "y": [y1, y2, y3, ... , yT], (Reference model simulation)
         "y_pred": [y1, y2, y3, ... , yT] (Predicted model simulation)
      }, 
         ... , 
      { 
         "output": "output_name",
         "unit": string, 
         "c": [low, high] (double),
         "y": [y1, y2, y3, ... , yT], (Reference model simulation)
         "y_pred": [y1, y2, y3, ... , yT] (Predicted model simulation)
      }
   ],

 "MV": [ 
      {
         "input": "input_name",
         "unit": string, 
         "c": [low, high], (double, constraining u)
         "u": [u1, u2, u3, ... , uT] 
      }, 
         ... , 
      { 
         "input": "input_name",
         "unit": string, 
         "c": [low, high], (double, constraining u)
         "u": [u1, u2, u3, ... , uT] 
      }
   ]
}
``` 