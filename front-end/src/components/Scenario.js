import React, { useState } from "react";
import { TextField, Box, Typography, Button } from "@mui/material";

import 'katex/dist/katex.min.css';
import { BlockMath } from 'react-katex';

const DataTypes = ["double", "vector<double>", "string", "int"];
const TextFields = { "System": "", "Scenario": "", "T": 0, "P": 0, "M": 0, "W": 0, "Q": "[ ]", "R": "[ ]", "RoH": "[ ]", "RoL": "[ ]", "Lower": "[ ]", "Upper": "[ ]"};
const Formulas = [`\\leq \\Delta U \\leq`, `\\leq U \\leq`, `\\leq Y \\leq`];

export default function Scenario() {

    const keys = Object.keys(TextFields); // Access keys
    //const items = Object.items(TextFields); // Access keys

    // Initialize objects to hold hooks
    const [valueStates, setValueStates] = useState(TextFields);

    // Declare state handlers 
    const handleSimulatonClick = e => {
        console.log("Simulation button clicked");
        // Run simulation 
    };

    const handleTextField = e => { // Update value in text field
        var newState = {};
        newState[e.target.id] = e.target.value;
        setValueStates(newState);
    }

    return (
        <Box sx={{ width: "100%", pt: 3, pl: "2%" }}>
            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <TextField id={keys[0]} variant="outlined" helperText={"System name, "+ DataTypes[2]} value={valueStates[keys[0]]} onChange={handleTextField} required/>
                <TextField id={keys[1]} variant="outlined" helperText={"Scenario name, " + DataTypes[2]} value={valueStates[keys[1]]} onChange={handleTextField} required/>
                <Box sx={{pl: 2}}/>
                <TextField id={keys[2]} variant="outlined" helperText={"MPC horizon T, " + DataTypes[3]} value={valueStates[keys[2]]} onChange={handleTextField} required/>
                <Box sx={{pl: "2%"}}/>
                <Button variant="contained" size="large" color="success" onClick={handleSimulatonClick}> RUN SIMULATION </Button>
            </Box>

            <Box align="left" sx={{pl: 30, pt: 2}}>
                <Typography variant="h5" sx={{fontWeight: 'bold'}}> Model Predictive Controller: </Typography>
            </Box>
           
            <Box sx={{pl: 6, pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:2, pl: 10}}>
                    <TextField id={keys[3]} variant="outlined" helperText={"Prediction horizon P, " + DataTypes[0]} value={valueStates[keys[3]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[4]} variant="outlined" helperText={"Control horizon M, " + DataTypes[0]} value={valueStates[keys[4]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[5]} variant="outlined" helperText={"Time delay, W " + DataTypes[0]} value={valueStates[keys[5]]} onChange={handleTextField} required/>
                </Box>

                <Box sx={{pt:2, pl: 5}}>
                    <TextField id={keys[6]} variant="outlined" helperText={"Q, " + DataTypes[1]} value={valueStates[keys[6]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[7]} variant="outlined" helperText={"R, " +DataTypes[1]} value={valueStates[keys[7]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[8]} variant="outlined" helperText={"RoH, " +DataTypes[0]} value={valueStates[keys[8]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[9]} variant="outlined" helperText={"RoL, " +DataTypes[0]} value={valueStates[keys[9]]} onChange={handleTextField} required/>
                </Box>
            </Box>

            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:6, pl: 10}}>
                    <TextField id={keys[10]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[10]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[10]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[10]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[10]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[10]]} onChange={handleTextField} required/>
                </Box>

                <Box >
                <Typography variant="h5" align="center" sx={{fontWeight: 'bold'}}> Constraints: </Typography>
                <div className="katex"> 
                    <Box sx={{pt: 1}}/>
                    <BlockMath math={Formulas[0]} />
                    <Box sx={{pt: 1}}/>
                    <BlockMath math={Formulas[1]} />
                    <Box sx={{pt: 1}}/>
                    <BlockMath math={Formulas[2]} />
                </div>
                </Box >
                    
                <Box sx ={{pt: 6}}>
                    <TextField id={keys[11]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[11]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[11]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[11]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[11]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[11]]} onChange={handleTextField} required/>
                </Box>
            </Box>
        </Box>
    );
}

