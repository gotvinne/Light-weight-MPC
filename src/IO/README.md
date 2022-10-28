
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
         "init": float,
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
         "init": float,
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
         "init": float,
         "unit": string
      },
         ... , 
      {
         "input": "input_name",
         "init": float,
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
 "CV": [ 
      {  // This is only the predicted states, the simulation uses a model.
         "output": "output_name",
         "unit": string, 
         "y[1]": [y1, y2, y3, ... , yT], (Reference model simulation)
         "y_hat[1]": [y1, y2, y3, ... , yT] (Predicted model simulation)
      }, 
         ... , 
      { 
         "output": "output_name",
         "unit": string, 
         "y[n_CV]": [y1, y2, y3, ... , yT], (Reference model simulation)
         "y_hat[n_CV]": [y1, y2, y3, ... , yT] (Predicted model simulation)
      }
   ],

 "MV": [ 
      {
         "input": "input_name",
         "unit": string, 
         "u[1] ": [u1, u2, u3, ... , uT] 
      }, 
         ... , 
      { 
         "input": "input_name",
         "unit": string, 
         "u[n_MV]": [u1, u2, u3, ... , uT] 
      }
   ]
}
``` 