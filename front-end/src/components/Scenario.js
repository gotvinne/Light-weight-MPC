import React, { useState } from "react";
import { TextField, Box, Typography, Button } from "@mui/material";

import 'katex/dist/katex.min.css';
import { BlockMath } from 'react-katex';

const DataTypes = ["double", "vector<double>", "string", "int"];
const TextFields = { "System": "", "Scenario": "", "T": 0, "P": 0, "M": 0, "W": 0, "Q": [], "R": [], "RoH": [], "RoL": [], "Lower": [], "Upper": []};
const Formulas = [`\\leq \\Delta U \\leq`, `\\leq U \\leq`, `\\leq Y \\leq`];


export default function Scenario() {

    const keys = Object.keys(TextFields); // Access keys
    const [value, setValue] = useState();

    // Declare state handlers 
    const handleSimulatonClick = e => {
        console.log("Simulation button clicked");
    };

    const handleTextField = e => {
        setValue(e.target.value); // Updating value variable
        console.log("Textfield " + value);
    };

    return (
        <Box sx={{ width: "100%", pt: 3, pl: "2%" }}>
            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <TextField id="system" label={keys[0]} variant="outlined" helperText={DataTypes[2]} value={value} onChange={handleTextField} required/>
                <TextField id="scenario" label={keys[1]} variant="outlined" helperText={DataTypes[2]} required/>
                <Box sx={{pl: 2}}/>
                <TextField id="T" label={keys[2]} variant="outlined" helperText={DataTypes[3]} required/>
                <Box sx={{pl: "2%"}}/>
                <Button variant="contained" size="large" color="success" onClick={handleSimulatonClick}> RUN SIMULATION </Button>
            </Box>

            <Box align="left" sx={{pl: 30, pt: 2}}>
                <Typography variant="h5" sx={{fontWeight: 'bold'}}> Model Predictive Controller: </Typography>
            </Box>
           
            <Box sx={{pl: 6, pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:2, pl: 10}}>
                    <TextField id="p" label={keys[3]} variant="outlined" helperText={DataTypes[0]} required/>
                    <Box />
                    <TextField id="m" label={keys[4]} variant="outlined" helperText={DataTypes[0]} required/>
                    <Box />
                    <TextField id="w" label={keys[5]} variant="outlined" helperText={DataTypes[0]} required/>
                </Box>

                <Box sx={{pt:2, pl: 5}}>
                    <TextField id="q" label={keys[6]} variant="outlined" helperText={DataTypes[1]} required/>
                    <Box />
                    <TextField id="R" label={keys[7]} variant="outlined" helperText={DataTypes[1]} required/>
                    <Box />
                    <TextField id="roh" label={keys[8]} variant="outlined" helperText={DataTypes[0]} required/>
                    <Box />
                    <TextField id="rol" label={keys[9]} variant="outlined" helperText={DataTypes[0]} required/>
                </Box>
            </Box>

            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:6, pl: 10}}>
                    <TextField id="lDu" label={keys[10]} variant="outlined" helperText={DataTypes[1]} required/>
                    <Box />
                    <TextField id="lU" label={keys[10]} variant="outlined" helperText={DataTypes[1]} required/>
                    <Box />
                    <TextField id="lY" label={keys[10]} variant="outlined" helperText={DataTypes[1]} required/>
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
                    <TextField id="uDu" label={keys[11]} variant="outlined" helperText={DataTypes[1]} required/>
                    <Box />
                    <TextField id="uU" label={keys[11]} variant="outlined" helperText={DataTypes[1]} required/>
                    <Box />
                    <TextField id="uY" label={keys[11]} variant="outlined" helperText={DataTypes[1]} required/>
                </Box>
            </Box>
        </Box>
    );
}

