import React from "react";
import { Box, Typography, TextField } from '@mui/material';
import { BlockMath } from 'react-katex';

const SYMBOLS = [`\\leq \\Delta U \\leq`, `\\leq U \\leq`, `\\leq Y \\leq`];

/**
 * Displaying the MPC constraint TextFields
 * @param {Number} nmv Number of manipulated variables
 * @param {Number} ncv Number of controlled variables
 * @param {React.useState} scescenario hook
 * @param {React.useState} error error hook
 * @param {Function} handler Callback function
 */
export default function Constraints({nmv, ncv, sce, error, handler}) {
    return (
        <div>
            <Box align="left" sx={{pl: "32%", pt: 2}}>
                <Typography variant="h5" sx={{fontWeight: 'bold'}}> Constraints: </Typography>
            </Box>
            <Box sx={{pl: "3%", display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:2}}>
                    <TextField error={error["ldu"]} sx={{width: "90%"}} id={"ldu"} variant="outlined" helperText={"vector<double>, length: " + nmv.toString()} value={sce["ldu"]} onChange={handler} required/>
                    <Box />
                    <TextField error={error["lu"]} sx={{width: "90%"}} id={"lu"} variant="outlined" helperText={"vector<double>, length: " + nmv.toString()} value={sce["lu"]} onChange={handler} required/>
                    <Box />
                    <TextField error={error["ly"]} sx={{width: "90%"}} id={"ly"} variant="outlined" helperText={"vector<double>, length: "  + ncv.toString()} value={sce["ly"]} onChange={handler} required/>
                </Box>

                <Box >
                    <div className="katex">
                        <Box sx={{pt: 1}}/>
                        <BlockMath math={SYMBOLS[0]} />
                        <Box sx={{pt: 1}}/>
                        <BlockMath math={SYMBOLS[1]} />
                        <Box sx={{pt: 1}}/>
                        <BlockMath math={SYMBOLS[2]} />
                    </div>
                </Box >
                    
                <Box sx ={{pt: 2}}>
                    <TextField error={error["udu"]} sx={{width: "90%"}} id={"udu"} variant="outlined" helperText={"vector<double>, length: " + nmv.toString()} value={sce["udu"]} onChange={handler} required/>
                    <Box />
                    <TextField error={error["uu"]} sx={{width: "90%"}} id={"uu"} variant="outlined" helperText={"vector<double>, length: " + nmv.toString()} value={sce["uu"]} onChange={handler} required/>
                    <Box />
                    <TextField error={error["uy"]} sx={{width: "90%"}} id={"uy"} variant="outlined" helperText={"vector<double>, length: " + ncv.toString()} value={sce["uy"]} onChange={handler} required/>
                </Box>
            </Box>
        </div>
    );
}