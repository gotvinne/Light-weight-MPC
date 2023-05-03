import React, { useState, useEffect, useMemo } from "react";
import { TextField, Box, Typography, Button, MenuItem, FormControl, InputLabel, Select } from "@mui/material";
import { BlockMath } from 'react-katex';
import { importSystems, readModelParams, readSystem, serializeRef, serializeScenario } from "../../utils/IO.js";
import { variableRender } from "../../utils/rendering.js";
import { updateError } from "../../utils/error.js";
import { simulate } from "../../utils/wasm.js";

import "../../css/Modules.css"

const LOCAL_STORAGE_KEY = 'lightweightMPC.storage';
 
const DATA_TYPES = ["int", "vector<double>", "string"];
const FORMULAS = [`\\leq \\Delta U \\leq`, `\\leq U \\leq`, `\\leq Y \\leq`];
//const TEXT_FIELDS = { "System": "", "Scenario": "test", "T": 180, "P": 100, "M": 50, "W": 0, "Q": "[1, 100]", "R": "[1, 100]", "RoH": "[1, 1]", "RoL": "[1, 1]", "ldu": "[-2, -10]", "lu": "[0, 0]", "ly": "[0, 0]", "udu": "[2, 10]", "uu": "[100, 1000]", "uy": "[4000, 100]"};
const TEXT_FIELDS = { "System": "", "Scenario": "", "T": 0, "P": 0, "M": 0, "W": 0, "Q": "[]", "R": "[]", "RoH": "[]", "RoL": "[]", "ldu": "[]", "lu": "[]", "ly": "[]", "udu": "[]", "uu": "[]", "uy": "[]"};
const ERROR = {"T": true, "P": true, "M": true, "W": true, "Q": true, "R": true, "RoH": true, "RoL": true, "ldu": true, "lu": true, "ly": true, "udu": true, "uu": true, "uy": true};
const KEYS = Object.keys(TEXT_FIELDS); 

/**
 * Module defining MPC scenario
 * @param {React.useState} simHook hook to store simulation data  
 */
export default function Scenario({simHook}) {
    //** HOOKS */
    const [sce, setSce] = useState(TEXT_FIELDS); // Initialize scenario is a dictionary of hooks
    const [systemNames] = useState(importSystems()); // System names
    const [error, setError] = useState(ERROR);
    const [ref, setRef] = useState([]);
        
    const [cv, mv] = useMemo(() => { // Read model params for displayment
        if (sce[KEYS[0]] === "") {
            return [[], []];
        } else {
            return [readModelParams(sce[KEYS[0]], "CV"), readModelParams(sce[KEYS[0]], "MV")];
        }
    }, [sce[KEYS[0]]]);

    //** USE EFFECTS */
    useEffect(() => { // Fetch storage
        const storedSce = JSON.parse(localStorage.getItem(LOCAL_STORAGE_KEY));
        if (storedSce !== null) setSce(storedSce);
        
    }, []);

    useEffect(() => { // Store sce
        if (sce !== TEXT_FIELDS) {
            localStorage.setItem(LOCAL_STORAGE_KEY, JSON.stringify(sce));
        }
        setError(updateError(sce, error, cv.length, mv.length));
    }, [sce])
    
    //** HANDLER FUNCTIONS */ 
    const handleSimulatonClick = () => { // Handle Simulation button

        // Check TextFields if valid inputs are given.

        const sys_file = readSystem(sce[KEYS[0]]);
        const sce_file = serializeScenario(sce);
        const ref_str = serializeRef(ref);

        // Might use async
        // MPC simulation:
        //simulate(sce_file, sys_file, sce[KEYS[1]], ref_str, parseInt(sce[KEYS[2]]), simHook);
    };

    const handleTextField = e => { // Handle input bars
        setSce(sce => {
            return {...sce, [e.target.id]: e.target.value}
        }); 
    }

    const handleReference = e => { // Handle reference bars
        setRef(reference => {
            reference[parseInt(e.target.id)] = parseFloat(e.target.value);
            return reference
        }); 
    }

    const handleSelect = e => { // Handle system select
        setSce(sce => {
            return {...sce, [KEYS[0]]: e.target.value}
        });
    };

    return (
        <div className="Scenario">
        <Box sx={{ width: "inherit", pt: 3, flexDirection: "row", display: "flex"}}>
           
            <Box sx={{width: "50%", pl: "2%"}}> 
                <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                    <FormControl style={{minWidth: "20%"}}>
                        <InputLabel id={KEYS[0]}> System name </InputLabel>
                        <Select
                            id={KEYS[0]}
                            value={sce[KEYS[0]]}
                            label="System name"
                            onChange={handleSelect} 
                        >   
                        {systemNames.map((course, index) => {
                            return (
                            <MenuItem value={course} key={index}> {course} </MenuItem>
                            )
                        })}
                        </Select>
                    </FormControl>
                
                    <TextField id={KEYS[1]} variant="outlined" helperText={"Scenario name, " + DATA_TYPES[2]} value={sce[KEYS[1]]} onChange={handleTextField} required/>
                    <Box sx={{pl: 2}}/>
                    <TextField error={error[KEYS[2]]} sx={{width: "20%"}} id={KEYS[2]} variant="outlined" helperText={"MPC horizon T, " + DATA_TYPES[0]} value={sce[KEYS[2]]} onChange={handleTextField} required/>
                    <Box sx={{pl: "3%"}}/>
                    <Button variant="contained" size="large" color="success" onClick={handleSimulatonClick}> RUN SIMULATION </Button>
                </Box>
                <Box align="left" sx={{pl: "16%", pt: 2}}>
                    <Typography variant="h5" sx={{fontWeight: 'bold'}}> Model Predictive Controller: </Typography>
                </Box>
            
                <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                    <Box sx ={{pt:2, pl: 7}}>
                        <TextField error={error[KEYS[3]]} sx={{width: "70%"}} id={KEYS[3]} variant="outlined" helperText={"Prediction horizon P, " + DATA_TYPES[0]} value={sce[KEYS[3]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField error={error[KEYS[4]]} sx={{width: "70%"}} id={KEYS[4]} variant="outlined" helperText={"Control horizon M, " + DATA_TYPES[0]} value={sce[KEYS[4]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField error={error[KEYS[5]]} sx={{width: "70%"}} id={KEYS[5]} variant="outlined" helperText={"Time delay, W " + DATA_TYPES[0]} value={sce[KEYS[5]]} onChange={handleTextField} required/>
                    </Box>

                    <Box sx={{pt:2}}>
                        <TextField error={error[KEYS[6]]} sx={{width: "90%"}} id={KEYS[6]} variant="outlined" helperText={"Q, " + DATA_TYPES[1]+", length: " + cv.length.toString()} value={sce[KEYS[6]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField error={error[KEYS[7]]} sx={{width: "90%"}} id={KEYS[7]} variant="outlined" helperText={"R, " +DATA_TYPES[1]+", length: " + mv.length.toString()} value={sce[KEYS[7]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField error={error[KEYS[8]]} sx={{width: "90%"}} id={KEYS[8]} variant="outlined" helperText={"RoH, " +DATA_TYPES[1]+", length: " + cv.length.toString()} value={sce[KEYS[8]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField error={error[KEYS[9]]} sx={{width: "90%"}} id={KEYS[9]} variant="outlined" helperText={"RoL, " +DATA_TYPES[1]+", length: " + cv.length.toString()} value={sce[KEYS[9]]} onChange={handleTextField} required/>
                    </Box>
                </Box>
                <Box align="left" sx={{pl: "24%", pt: 2}}>
                    <Typography variant="h5" sx={{fontWeight: 'bold'}}> Constraints: </Typography>
                </Box>
                <Box sx={{display: "flex", flexDirection: "row"}}>
                    <Box sx ={{pt:2}}>
                        <TextField error={error[KEYS[10]]} sx={{width: "90%"}} id={KEYS[10]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + mv.length.toString()} value={sce[KEYS[10]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField error={error[KEYS[11]]} sx={{width: "90%"}} id={KEYS[11]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + mv.length.toString()} value={sce[KEYS[11]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField error={error[KEYS[12]]} sx={{width: "90%"}} id={KEYS[12]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + cv.length.toString()} value={sce[KEYS[12]]} onChange={handleTextField} required/>
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
                        <TextField error={error[KEYS[13]]} sx={{width: "90%"}} id={KEYS[13]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + mv.length.toString()} value={sce[KEYS[13]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField error={error[KEYS[14]]} sx={{width: "90%"}} id={KEYS[14]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + mv.length.toString()} value={sce[KEYS[14]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField error={error[KEYS[15]]} sx={{width: "90%"}} id={KEYS[15]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + cv.length.toString()} value={sce[KEYS[15]]} onChange={handleTextField} required/>
                    </Box>
                </Box>
            </Box>

            <Box sx={{width: "50%"}}>
                <Box sx={{pt: 2, height: "5%", display: "flex", flexDirection: "row"}} >
                    <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                        {variableRender(cv.length, "CV")}
                    </Box>
                </Box>
                <Box sx={{pt: 2, height: "30%", display: "flex", flexDirection: "row" }} >
                    <Box sx={{width: "15%", pt: 2, display: "flex", flexDirection: "row"}}> 
                    </Box>
                    <Box sx={{width: "20%"}}> 
                        {cv.map((course, index) => {
                            return (
                            <Box key={index} sx={{width: "80%", height: "27%", pt: 2}}> 
                                <Typography variant="h5" key={index}> {course + ":"} </Typography>
                            </Box>
                            )
                        })}
                    </Box>
                    <Box sx={{width: "35%"}}> 
                        {cv.map((course, index) => {
                            return (
                            <Box key={index} sx={{width: "80%"}}> 
                                <TextField id={index.toString()} variant="outlined" helperText={course + " reference, " + DATA_TYPES[0]} value={ref[index]} onChange={handleReference} required/>
                            </Box>
                            )
                        })}
                    </Box>
                </Box>
                <Box sx={{pt: 2, height: "5%", display: "flex", flexDirection: "row" }} >
                    <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                        {variableRender(cv.length, "MV")}
                    </Box>
                </Box>
                <Box sx={{pt: 2, height: "30%", display: "flex", flexDirection: "row" }} >
                    <Box sx={{width: "15%", pt: 2, display: "flex", flexDirection: "row"}}> 
                    </Box>
                    <Box sx={{width: "20%"}}> 
                        {mv.map((course, index) => {
                            return (
                            <Box key={index} sx={{width: "80%", height: "27%", pt: 2}}> 
                                <Typography variant="h5" key={index}> {course} </Typography>
                            </Box>
                            )
                        })}
                    </Box>
                </Box>
            </Box>
        </Box>
        </div>
    );
}
