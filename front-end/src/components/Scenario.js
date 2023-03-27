import React from "react";
import { TextField, Box, Typography, Button } from "@mui/material";

import 'katex/dist/katex.min.css';
import { BlockMath } from 'react-katex';

const DataTypes = ["double", "vector<double>", "String", "int"];
const Buttons = ["System name", "Prediction horizon, P", "Control Horizon, M", "Time delay, W", "Q", "R", "RoH", "RoL", "T", "Scenario name"];
const Constraints = ["Lower", "Upper"];
const Formulas = [`\\leq \\Delta U \\leq`, `\\leq U \\leq`, `\\leq Y \\leq`];

export default function Scenario() {
    return (
        <Box sx={{ width: "100%", pt: 3, pl: "2%" }}>
            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <TextField id="system" label={Buttons[0]} variant="outlined" helperText={DataTypes[2]} />
                <TextField id="system" label={Buttons[9]} variant="outlined" helperText={DataTypes[2]} />
                <Box sx={{pl: 2}}/>
                <TextField id="T" label={Buttons[8]} variant="outlined" helperText={DataTypes[3]} />
                <Box sx={{pl: "2%"}}/>
                <Button variant="contained" size="large" color="success">RUN SIMULATION</Button>
            </Box>

            <Box align="left" sx={{pl: 30, pt: 2}}>
                <Typography variant="h5" sx={{fontWeight: 'bold'}}> Model Predictive Controller: </Typography>
            </Box>
           
            <Box sx={{pl: 6, pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:2, pl: 10}}>
                    <TextField id="p" label={Buttons[1]} variant="outlined" helperText={DataTypes[0]}/>
                    <Box />
                    <TextField id="m" label={Buttons[2]} variant="outlined" helperText={DataTypes[0]}/>
                    <Box />
                    <TextField id="w" label={Buttons[3]} variant="outlined" helperText={DataTypes[0]}/>
                </Box>

                <Box sx={{pt:2, pl: 5}}>
                    <TextField id="q" label={Buttons[4]} variant="outlined" helperText={DataTypes[1]}/>
                    <Box />
                    <TextField id="R" label={Buttons[5]} variant="outlined" helperText={DataTypes[1]}/>
                    <Box />
                    <TextField id="roh" label={Buttons[6]} variant="outlined" helperText={DataTypes[0]}/>
                    <Box />
                    <TextField id="rol" label={Buttons[7]} variant="outlined" helperText={DataTypes[0]}/>
                </Box>
            </Box>

            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:6, pl: 10}}>
                    <TextField id="p" label={Constraints[0]} variant="outlined" helperText={DataTypes[0]}/>
                    <Box />
                    <TextField id="m" label={Constraints[0]} variant="outlined" helperText={DataTypes[0]}/>
                    <Box />
                    <TextField id="w" label={Constraints[0]} variant="outlined" helperText={DataTypes[0]}/>
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
                    <TextField id="p" label={Constraints[1]} variant="outlined" helperText={DataTypes[0]}/>
                    <Box />
                    <TextField id="m" label={Constraints[1]} variant="outlined" helperText={DataTypes[0]}/>
                    <Box />
                    <TextField id="w" label={Constraints[1]} variant="outlined" helperText={DataTypes[0]}/>
                </Box>
            </Box>
        </Box>
    );
}

