import React from "react"
import { Box, Typography, TextField } from '@mui/material'
import { InlineMath } from 'react-katex';

/**
 * Render variables in mathematical format
 * @param {Number} length 
 * @param {String} identifier n_CV / n_MV
 * @returns Mathematical rendering of length and identifier
 */
export function variableRender(length, identifier) {
    if (length === 0) {
        return (<Typography variant="h5">
             <InlineMath math={`n_{${identifier}} :`}/> 
            </Typography> );
    } else {
        return (<Typography variant="h5"> 
            <InlineMath math={`n_{${identifier}} = ${length}:`}/> 
            </Typography> );
    }
}

/**
 * Display Reference TextFields
 * @param {Array} cv cv_data
 * @param {Array} mv mv_data
 * @param {React.useState} ref_value reference hook
 * @param {number} n model horizon
 * @param {Function} handler callback function
 */
export default function Reference({cv, mv, ref_value, n, handler}) {
    return (
        <div>
            <Typography variant="h5" sx={{fontWeight: 'bold'}} >
                Reference(s): 
            </Typography>
            <Box sx={{height: "5%", display: "flex", flexDirection: "row"}} >
                <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                    {variableRender(cv[0].length, "CV")}
                </Box>
            </Box>
            <Box sx={{pt: 2, height: "35%", display: "flex", flexDirection: "row" }} >
                <Box sx={{width: "15%", pt: 2, display: "flex", flexDirection: "row"}}> 
                </Box>
                <Box sx={{width: "25%"}}> 
                    {cv[0].map((course, index) => {
                        return (
                        <Box key={index} sx={{width: "80%", height: "27%", pt: 2}}> 
                            <Typography variant="h5" key={index}> {course + ":"} </Typography>
                        </Box>
                        )
                    })}
                </Box>
                <Box sx={{width: "35%"}}> 
                    {cv[0].map((course, index) => {
                        return (
                        <Box key={index}> 
                            <TextField sx={{width: "90%"}} id={index.toString()} variant="outlined" helperText={course + " reference, int"} value={ref_value[index]} onChange={handler} required/>
                        </Box>
                        )
                    })}
                </Box>
                <Box sx={{width: "20%"}}> 
                    {cv[1].map((unit, index) => {
                        return (
                        <Box key={index} sx={{width: "80%", height: "27%", pt: 2}}> 
                            <Typography variant="h5" key={index}> <InlineMath math={unit} /> </Typography>
                        </Box>
                        )
                    })}
                </Box>
            </Box>
            <Box sx={{pt: 2, height: "5%", display: "flex", flexDirection: "row" }} >
                <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                    {variableRender(mv[0].length, "MV")}
                </Box>
            </Box>
            <Box sx={{pt: 2, height: "30%", display: "flex", flexDirection: "row" }} >
                <Box sx={{width: "15%", pt: 2, display: "flex", flexDirection: "row"}}> 
                </Box>
                <Box sx={{width: "50%"}}> 
                    {mv[0].map((course, index) => {
                        return (
                        <Box key={index} sx={{width: "80%", height: "27%", pt: 2}}> 
                            <Typography variant="h5" key={index}> {course + ": "} <InlineMath math={mv[1][index]} /> </Typography>
                        </Box>
                        )
                    })}
                </Box>
            </Box>
            <Box sx={{height: "5%", display: "flex", flexDirection: "row"}} >
                <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                    <Typography variant="h5">
                        <InlineMath math={`N_* = ${n}`} />
                    </Typography>
                </Box>
            </Box>
        </div>
    );
}
