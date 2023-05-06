import React, { useEffect, useState } from "react"
import { Box, Typography, TextField } from '@mui/material'
import { variableRender } from "../../../utils/rendering.js";
import { InlineMath } from 'react-katex';

/**
 * Display Reference TextFields
 * @param {*} param0 
 */
export default function Reference({cv, mv, value, handler}) {

    return (
        <Box sx={{width: "45%"}}>
            <Box sx={{pt: 2, height: "5%", display: "flex", flexDirection: "row"}} >
                <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                    {variableRender(cv[0].length, "CV")}
                </Box>
            </Box>
            <Box sx={{pt: 2, height: "30%", display: "flex", flexDirection: "row" }} >
                <Box sx={{width: "15%", pt: 2, display: "flex", flexDirection: "row"}}> 
                </Box>
                <Box sx={{width: "20%"}}> 
                    {cv[0].map((course, index) => {
                        return (
                        <Box key={index} sx={{width: "80%", height: "27%", pt: 2}}> 
                            <Typography variant="h5" key={index}> {course + ":"} </Typography>
                        </Box>
                        )
                    })}
                </Box>
                <Box sx={{width: "30%"}}> 
                    {cv[0].map((course, index) => {
                        return (
                        <Box key={index}> 
                            <TextField sx={{width: "90%"}} id={index.toString()} variant="outlined" helperText={course + " reference, int"} value={value[index]} onChange={handler} required/>
                        </Box>
                        )
                    })}
                </Box>
                <Box sx={{width: "15%"}}> 
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
                <Box sx={{width: "35%"}}> 
                    {mv[0].map((course, index) => {
                        return (
                        <Box key={index} sx={{width: "80%", height: "27%", pt: 2}}> 
                            <Typography variant="h5" key={index}> {course + ": "} <InlineMath math={mv[1][index]} /> </Typography>
                        </Box>
                        )
                    })}
                </Box>
            </Box>
        </Box>
    );
}
