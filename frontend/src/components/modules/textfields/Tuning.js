import React from "react";
import { Box, Typography, TextField } from '@mui/material';

/**
 * Displaying the MPC tuning TextFields
 * @param {Number} ncv Number of controlled variables
 * @param {Number} nmv Number of manipulated variables  
 * @param {React.useState} error hook
 * @param {React.useState} sce hook
 * @param {Function} handler callback function
 */
export default function Tuning({ncv, nmv, error, sce, handler}) {
    return (
        <div> 
            <Box align="left" sx={{pl: "22%", pt: 2}}>
                <Typography variant="h5" sx={{fontWeight: 'bold'}}> Model Predictive Controller: </Typography>
            </Box>
        
            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:2, pl: "10%"}}>
                    <TextField error={error["P"]} sx={{width: "90%"}} id={"P"} variant="outlined" helperText={"Prediction horizon P, int"} value={sce["P"]} onChange={handler} required/>
                    <Box />
                    <TextField error={error["M"]} sx={{width: "90%"}} id={"M"} variant="outlined" helperText={"Control horizon M, int"} value={sce["M"]} onChange={handler} required/>
                    <Box />
                    <TextField error={error["W"]} sx={{width: "90%"}} id={"W"} variant="outlined" helperText={"Time delay, W, int"} value={sce["W"]} onChange={handler} required/>
                </Box>

                <Box sx={{pt:2}}>
                    <TextField error={error["Q"]} sx={{width: "90%"}} id={"Q"} variant="outlined" helperText={"Q, vector<double>, length: "+ ncv.toString()} value={sce["Q"]} onChange={handler} required/>
                    <Box />
                    <TextField error={error["R"]} sx={{width: "90%"}} id={"R"} variant="outlined" helperText={"R, vector<double>, length: "+ nmv.toString()} value={sce["R"]} onChange={handler} required/>
                    <Box />
                    <TextField error={error["RoH"]} sx={{width: "90%"}} id={"RoH"} variant="outlined" helperText={"RoH, vector<double>, length: "+ ncv.toString()} value={sce["RoH"]} onChange={handler} required/>
                    <Box />
                    <TextField error={error["RoL"]} sx={{width: "90%"}} id={"RoL"} variant="outlined" helperText={"RoL, vector<double>, length: "+ ncv.toString()} value={sce["RoL"]} onChange={handler} required/>
                </Box>
            </Box>
        </div>
    );
}