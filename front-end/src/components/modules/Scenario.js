import React, { useState, useEffect, useMemo } from "react";
import { TextField, Box, Typography, Button, MenuItem, FormControl, InputLabel, Select } from "@mui/material";

import 'katex/dist/katex.min.css';
import { BlockMath } from 'react-katex';
import { importSystems, readModelParams, readSystem, serializeScenario } from "../../utils/IO.js";
import { variableRender } from "../../utils/rendering.js";
import "../../css/Modules.css"
//import backend from "../../webassembly.mjs";

const LOCAL_STORAGE_KEY = 'lightweightMPC.storage';

const DATA_TYPES = ["double", "vector<double>", "string", "int"];
const FORMULAS = [`\\leq \\Delta U \\leq`, `\\leq U \\leq`, `\\leq Y \\leq`];
const TEXT_FIELDS = { "System": "", "Scenario": "", "T": 0, "P": 0, "M": 0, "W": 0, "Q": "[ ]", "R": "[ ]", "RoH": "[ ]", "RoL": "[ ]", "ldu": "[ ]", "lu": "[ ]", "ly": "[ ]", "udu": "[ ]", "uu": "[ ]", "uy": "[ ]"};
const KEYS = Object.keys(TEXT_FIELDS); // Access keys

export default function Scenario(props) {
    //** HOOKS */
    const [tuning, setTuning] = useState(TEXT_FIELDS); // Initialize tuning is a dictionary of hooks
    const [scenario, setScenario] = useState(); // Scenario file
    const [system, setSystem] = useState(); // System file
    const [systemNames] = useState(importSystems()); // System names
    const [reference] = useState(0);
        
    const [ncv, nmv] = useMemo(() => {
        if (tuning[KEYS[0]] === "") {
            return [[], []];
        } else {
            return [readModelParams(tuning[KEYS[0]], "CV"), readModelParams(tuning[KEYS[0]], "MV")];
        }
    }, [tuning[KEYS[0]]]);

    //** USE EFFECTS */
    useEffect(() => { // Called for every rerender. 
        const storedtuning = JSON.parse(localStorage.getItem(LOCAL_STORAGE_KEY));
        if (storedtuning !== null) setTuning(storedtuning);
    }, []);

    useEffect(() => { // Store tuning
        if (tuning !== TEXT_FIELDS) localStorage.setItem(LOCAL_STORAGE_KEY, JSON.stringify(tuning))
    }, [tuning])
    
    //** HANDLER FUNCTIONS */ 
    const handleSimulatonClick = () => {
        readSystem(tuning[KEYS[0]], setSystem);
        serializeScenario(tuning, setScenario);
        //let wasm: any;
        //backend()
         //   .then((module) => {
          //      wasm = module
          //      setScenario(wasm.sayHello(JSON.stringify(tuning)));
          //      })
          //  .catch(setScenario)

        //props.moduleHook(props.modules["simulation"]); // Change page
    };

    const handleTextField = e => { 
        setTuning(tuning => {
            return {...tuning, [e.target.id]: e.target.value}
        }); 
    }

    //const handleReference = e => { 
      //  const update = {};
        //update[e.target.id] = e.target.value;
        //setReference(reference => {
        //    return {...reference, ...update}
        //}); 
    //}

    const handleSelect = e => {
        setTuning(tuning => {
            return {...tuning, [KEYS[0]]: e.target.value}
        });
    };

    return (
        <div className="Scenario">
        <Box sx={{ width: "inherit", pt: 3, pl: "2%", flexDirection: "row", display: "flex"}}>
           
            <Box sx={{width: "50%"}}> 
                <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                    <FormControl style={{minWidth: "20%"}}>
                        <InputLabel id={KEYS[0]}> System name </InputLabel>
                        <Select
                            id={KEYS[0]}
                            value={tuning[KEYS[0]]}
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
                
                    <TextField id={KEYS[1]} variant="outlined" helperText={"Scenario name, " + DATA_TYPES[2]} value={tuning[KEYS[1]]} onChange={handleTextField} required/>
                    <Box sx={{pl: 2}}/>
                    <TextField id={KEYS[2]} variant="outlined" helperText={"MPC horizon T, " + DATA_TYPES[3]} value={tuning[KEYS[2]]} onChange={handleTextField} required/>
                    <Box sx={{pl: "2%"}}/>
                    <Button variant="contained" size="large" color="success" onClick={handleSimulatonClick}> RUN SIMULATION </Button>
                </Box>
                <Box align="left" sx={{pl: 30, pt: 2}}>
                    <Typography variant="h5" sx={{fontWeight: 'bold'}}> Model Predictive Controller: </Typography>
                </Box>
            
                <Box sx={{pl: 6, pt: 2, display: "flex", flexDirection: "row"}}>
                    <Box sx ={{pt:2, pl: 10}}>
                        <TextField id={KEYS[3]} variant="outlined" helperText={"Prediction horizon P, " + DATA_TYPES[0]} value={tuning[KEYS[3]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={KEYS[4]} variant="outlined" helperText={"Control horizon M, " + DATA_TYPES[0]} value={tuning[KEYS[4]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={KEYS[5]} variant="outlined" helperText={"Time delay, W " + DATA_TYPES[0]} value={tuning[KEYS[5]]} onChange={handleTextField} required/>
                    </Box>

                    <Box sx={{pt:2, pl: 5}}>
                        <TextField id={KEYS[6]} variant="outlined" helperText={"Q, " + DATA_TYPES[1]+", length: " + ncv.length.toString()} value={tuning[KEYS[6]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={KEYS[7]} variant="outlined" helperText={"R, " +DATA_TYPES[1]+", length: " + nmv.length.toString()} value={tuning[KEYS[7]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={KEYS[8]} variant="outlined" helperText={"RoH, " +DATA_TYPES[0]+", length: " + ncv.length.toString()} value={tuning[KEYS[8]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={KEYS[9]} variant="outlined" helperText={"RoL, " +DATA_TYPES[0]+", length: " + ncv.length.toString()} value={tuning[KEYS[9]]} onChange={handleTextField} required/>
                    </Box>
                </Box>

                <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                    <Box sx ={{pt:6, pl: 10}}>
                        <TextField id={KEYS[10]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + nmv.length.toString()} value={tuning[KEYS[10]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={KEYS[11]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + nmv.length.toString()} value={tuning[KEYS[11]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={KEYS[12]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + ncv.length.toString()} value={tuning[KEYS[12]]} onChange={handleTextField} required/>
                    </Box>

                    <Box >
                    <Typography variant="h5" align="center" sx={{fontWeight: 'bold'}}> Constraints: </Typography>
                    <div className="katex"> 
                        <Box sx={{pt: 1}}/>
                        <BlockMath math={FORMULAS[0]} />
                        <Box sx={{pt: 1}}/>
                        <BlockMath math={FORMULAS[1]} />
                        <Box sx={{pt: 1}}/>
                        <BlockMath math={FORMULAS[2]} />
                    </div>
                    </Box >
                        
                    <Box sx ={{pt: 6}}>
                        <TextField id={KEYS[13]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + nmv.length.toString()} value={tuning[KEYS[13]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={KEYS[14]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + nmv.length.toString()} value={tuning[KEYS[14]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={KEYS[15]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + ncv.length.toString()} value={tuning[KEYS[15]]} onChange={handleTextField} required/>
                    </Box>
                </Box>
            </Box>

            <Box sx={{width: "40%", pt: 2}}>
                <Box sx={{pt: 2, height: "5%", display: "flex", flexDirection: "row" }} >
                    <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                        {variableRender(ncv.length, "CV")}
                    </Box>
                </Box>
                <Box sx={{pt: 2, height: "30%", display: "flex", flexDirection: "row" }} >
                    <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                    </Box>
                    <Box sx={{width: "25%"}}> 
                        {ncv.map((course, index) => {
                            return (
                            <Box key={index} sx={{width: "80%", height: "27%", pt: 2}}> 
                                <Typography variant="h5" key={index}> {course} </Typography>
                            </Box>
                            )
                        })}
                    </Box>
                    <Box sx={{width: "35%"}}> 
                        {ncv.map((course, index) => {
                            return (
                            <Box key={index} sx={{width: "80%"}}> 
                                <TextField id={course} variant="outlined" helperText={course + " reference, " + DATA_TYPES[0]} value={reference} required/>
                            </Box>
                            )
                        })}
                    </Box>
                </Box>
                <Box sx={{pt: 2, height: "5%", display: "flex", flexDirection: "row" }} >
                    <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                        {variableRender(ncv.length, "MV")}
                    </Box>
                </Box>
                <Box sx={{pt: 2, height: "30%", display: "flex", flexDirection: "row" }} >
                    <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                    </Box>
                    <Box sx={{width: "25%"}}> 
                        {nmv.map((course, index) => {
                            return (
                            <Box key={index} sx={{width: "80%", height: "27%", pt: 2}}> 
                                <Typography variant="h5" key={index}> {course} </Typography>
                            </Box>
                            )
                        })}
                    </Box>
                    <Box sx={{width: "35%"}}> 
                        {nmv.map((course, index) => {
                            return (
                            <Box key={index} sx={{width: "80%"}}> 
                                <TextField id={course} variant="outlined" helperText={course + " reference, " + DATA_TYPES[0]} value={reference} required/>
                            </Box>
                            )
                        })}
                    </Box>
                </Box>
                

                <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                    <Typography> {scenario} </Typography>
                </Box>
            </Box>
        </Box>
        </div>
    );
}

