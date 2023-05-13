import React from "react";
import { Box, Typography, TextField } from '@mui/material';
import { BlockMath } from 'react-katex';

const FORMULAS = [`\\leq \\Delta U \\leq`, `\\leq U \\leq`, `\\leq Y \\leq`];

/**
 * Displaying the MPC constraint TextFields
 * @param {*} param0 
 */
export default function Constraints({keys, nmv, ncv, scenario, error, handler}) {
    return (
        <div>
            <Box align="left" sx={{pl: "32%", pt: 2}}>
                <Typography variant="h5" sx={{fontWeight: 'bold'}}> Constraints: </Typography>
            </Box>
            <Box sx={{pl: "3%", display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:2}}>
                    <TextField error={error[keys[10]]} sx={{width: "90%"}} id={keys[10]} variant="outlined" helperText={"vector<double>, length: " + nmv.toString()} value={scenario[keys[10]]} onChange={handler} required/>
                    <Box />
                    <TextField error={error[keys[11]]} sx={{width: "90%"}} id={keys[11]} variant="outlined" helperText={"vector<double>, length: " + nmv.toString()} value={scenario[keys[11]]} onChange={handler} required/>
                    <Box />
                    <TextField error={error[keys[12]]} sx={{width: "90%"}} id={keys[12]} variant="outlined" helperText={"vector<double>, length: "  + ncv.toString()} value={scenario[keys[12]]} onChange={handler} required/>
                </Box>

                <Box >
                    <div className="katex">
                        <Box sx={{pt: 1}}/>
                        <BlockMath math={FORMULAS[0]} />
                        <Box sx={{pt: 1}}/>
                        <BlockMath math={FORMULAS[1]} />
                        <Box sx={{pt: 1}}/>
                        <BlockMath math={FORMULAS[2]} />
                    </div>
                </Box >
                    
                <Box sx ={{pt: 2}}>
                    <TextField error={error[keys[13]]} sx={{width: "90%"}} id={keys[13]} variant="outlined" helperText={"vector<double>, length: " + nmv.toString()} value={scenario[keys[13]]} onChange={handler} required/>
                    <Box />
                    <TextField error={error[keys[14]]} sx={{width: "90%"}} id={keys[14]} variant="outlined" helperText={"vector<double>, length: " + nmv.toString()} value={scenario[keys[14]]} onChange={handler} required/>
                    <Box />
                    <TextField error={error[keys[15]]} sx={{width: "90%"}} id={keys[15]} variant="outlined" helperText={"vector<double>, length: " + ncv.toString()} value={scenario[keys[15]]} onChange={handler} required/>
                </Box>
            </Box>
        </div>
    );
}