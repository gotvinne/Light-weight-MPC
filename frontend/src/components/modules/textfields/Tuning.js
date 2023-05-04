import React from "react";
import { Box, Typography, TextField } from '@mui/material';

/**
 * Displaying the MPC tuning TextFields
 * @param {*} param0 
 */
export default function Tuning({keys, ncv, nmv, error, scenario, handler}) {
    return (
        <div> 
            <Box align="left" sx={{pl: "16%", pt: 2}}>
                <Typography variant="h5" sx={{fontWeight: 'bold'}}> Model Predictive Controller: </Typography>
            </Box>
        
            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:2, pl: 7}}>
                    <TextField error={error[keys[3]]} sx={{width: "90%"}} id={keys[3]} variant="outlined" helperText={"Prediction horizon P, int"} value={scenario[keys[3]]} onChange={handler} required/>
                    <Box />
                    <TextField error={error[keys[4]]} sx={{width: "90%"}} id={keys[4]} variant="outlined" helperText={"Control horizon M, int"} value={scenario[keys[4]]} onChange={handler} required/>
                    <Box />
                    <TextField error={error[keys[5]]} sx={{width: "90%"}} id={keys[5]} variant="outlined" helperText={"Time delay, W, int"} value={scenario[keys[5]]} onChange={handler} required/>
                </Box>

                <Box sx={{pt:2}}>
                    <TextField error={error[keys[6]]} sx={{width: "90%"}} id={keys[6]} variant="outlined" helperText={"Q, vector<double>, length: "+ ncv.toString()} value={scenario[keys[6]]} onChange={handler} required/>
                    <Box />
                    <TextField error={error[keys[7]]} sx={{width: "90%"}} id={keys[7]} variant="outlined" helperText={"R, vector<double>, length: "+ nmv.toString()} value={scenario[keys[7]]} onChange={handler} required/>
                    <Box />
                    <TextField error={error[keys[8]]} sx={{width: "90%"}} id={keys[8]} variant="outlined" helperText={"RoH, vector<double>, length: "+ ncv.toString()} value={scenario[keys[8]]} onChange={handler} required/>
                    <Box />
                    <TextField error={error[keys[9]]} sx={{width: "90%"}} id={keys[9]} variant="outlined" helperText={"RoL, vector<double>, length: "+ ncv.toString()} value={scenario[keys[9]]} onChange={handler} required/>
                </Box>
            </Box>
        </div>
    );
}