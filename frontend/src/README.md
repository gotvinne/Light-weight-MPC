# create-react-app, React, MaterialUI, css

The source code for the Front-end interface. 
## Components: 
- modules/plotly, plot components
- modules/textFields, TextField components for input
- Footer 
- Header
- Modules, Defining the different menu items
- TabPanel, switching between menu items

## Modules: 
- Scenario
- Simulation
- Algorithm
- Models
- About
- LoadingCircle

## Systems
Local database holding system description. 

NB! If a new system is added, update *components/modules/default_tuning.json* with the system discription.

## Webassembly
Simulation functionality is defined by mpc_core.mjs. This is emscripten compiled C/C++ code using Webassembly instruction format. This format allows other programming language to run on the web without losing performance. The wasm format is often targeted computational demanding web applications.
