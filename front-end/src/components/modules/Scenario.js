import React, { useState, useEffect } from "react";
import { TextField, Box, Typography, Button, MenuItem, FormControl, InputLabel } from "@mui/material";
import Select, { SelectChangeEvent } from '@mui/material/Select';

import 'katex/dist/katex.min.css';
import { BlockMath } from 'react-katex';
import { importSystems, readModelParams, readSystem, serializeScenario } from "../../utils/IO.js";
import { variableRender } from "../../utils/rendering.js";
import "../../css/Modules.css"
import LightWeightMPC from "../../web.mjs";

const LOCAL_STORAGE_KEY = 'lightweightMPC.storage';

const DATA_TYPES = ["double", "vector<double>", "string", "int"];
const FORMULAS = [`\\leq \\Delta U \\leq`, `\\leq U \\leq`, `\\leq Y \\leq`];
const TEXT_FIELDS = { "System": "", "Scenario": "", "T": 0, "P": 0, "M": 0, "W": 0, "Q": "[ ]", "R": "[ ]", "RoH": "[ ]", "RoL": "[ ]", "ldu": "[ ]", "lu": "[ ]", "ly": "[ ]", "udu": "[ ]", "uu": "[ ]", "uy": "[ ]"};

export default function Scenario(props) {
    const keys = Object.keys(TEXT_FIELDS); // Access keys
    
    //** HOOKS */
    const [valueStates, setValueStates] = useState(TEXT_FIELDS); // Initialize ValueStates is a dictionary of hooks
    const [scenario, setScenario] = useState(); // Scenario file
    const [system, setSystem] = useState(); // System file
    const [systemNames, setSystemNames] = useState([]); // System names
    
    const [ncv, setNCV] = useState([]);
    const [nmv, setNMV] = useState([]);

    //** USE EFFECTS */
    useEffect(() => { // Have no ide why this runs twice!
        importSystems(setSystemNames);

        const storedValueStates = JSON.parse(localStorage.getItem(LOCAL_STORAGE_KEY));
        if (storedValueStates !== null) setValueStates(storedValueStates);
    }, []);

    useEffect(() => { 

        if (valueStates[keys[0]] === "") {
            return;
        }
        // Call function to pop up system info
        readModelParams(valueStates[keys[0]], setNCV, "CV");
        readModelParams(valueStates[keys[0]], setNMV, "MV");
    }, [valueStates[keys[0]]]); // Every time this variable change

    useEffect(() => { // Store valueStates
        if (valueStates !== TEXT_FIELDS) localStorage.setItem(LOCAL_STORAGE_KEY, JSON.stringify(valueStates))
    }, [valueStates])
    
    //** HANDLER FUNCTIONS */ 
    const handleSimulatonClick = e => {
        readSystem(valueStates[keys[0]], setSystem);
        serializeScenario(valueStates, setScenario);
        //let wasm: any;
        //LightWeightMPC().then((module) => {
         //   wasm = module
         //   setScenario(wasm.sayHello(JSON.stringify(valueStates)));
        //});
        props.moduleHook(props.modules["simulation"]); // Change page
    };

    const handleTextField = e => { // Update react hook
        setValueStates(valueStates => ({...valueStates, [e.target.id]: e.target.value})); // Update dictionary hook
    }

    const handleSelect = (event: SelectChangeEvent) => {
        setValueStates(valueStates => ({...valueStates, [keys[0]]: event.target.value}));
    };

    return (
        <div className="Scenario">
        <Box sx={{ width: "inherit", pt: 3, pl: "2%", flexDirection: "row", display: "flex"}}>
           
            <Box sx={{width: "50%"}}> 
                <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                    <FormControl style={{minWidth: "20%"}}>
                        <InputLabel id={keys[0]}> System name </InputLabel>
                        <Select
                            id={keys[0]}
                            value={valueStates[keys[0]]}
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
                
                    <TextField id={keys[1]} variant="outlined" helperText={"Scenario name, " + DATA_TYPES[2]} value={valueStates[keys[1]]} onChange={handleTextField} required/>
                    <Box sx={{pl: 2}}/>
                    <TextField id={keys[2]} variant="outlined" helperText={"MPC horizon T, " + DATA_TYPES[3]} value={valueStates[keys[2]]} onChange={handleTextField} required/>
                    <Box sx={{pl: "2%"}}/>
                    <Button variant="contained" size="large" color="success" onClick={handleSimulatonClick}> RUN SIMULATION </Button>
                </Box>
                <Box align="left" sx={{pl: 30, pt: 2}}>
                    <Typography variant="h5" sx={{fontWeight: 'bold'}}> Model Predictive Controller: </Typography>
                </Box>
            
                <Box sx={{pl: 6, pt: 2, display: "flex", flexDirection: "row"}}>
                    <Box sx ={{pt:2, pl: 10}}>
                        <TextField id={keys[3]} variant="outlined" helperText={"Prediction horizon P, " + DATA_TYPES[0]} value={valueStates[keys[3]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={keys[4]} variant="outlined" helperText={"Control horizon M, " + DATA_TYPES[0]} value={valueStates[keys[4]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={keys[5]} variant="outlined" helperText={"Time delay, W " + DATA_TYPES[0]} value={valueStates[keys[5]]} onChange={handleTextField} required/>
                    </Box>

                    <Box sx={{pt:2, pl: 5}}>
                        <TextField id={keys[6]} variant="outlined" helperText={"Q, " + DATA_TYPES[1]+", length: " + ncv.length.toString()} value={valueStates[keys[6]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={keys[7]} variant="outlined" helperText={"R, " +DATA_TYPES[1]+", length: " + nmv.length.toString()} value={valueStates[keys[7]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={keys[8]} variant="outlined" helperText={"RoH, " +DATA_TYPES[0]+", length: " + ncv.length.toString()} value={valueStates[keys[8]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={keys[9]} variant="outlined" helperText={"RoL, " +DATA_TYPES[0]+", length: " + ncv.length.toString()} value={valueStates[keys[9]]} onChange={handleTextField} required/>
                    </Box>
                </Box>

                <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                    <Box sx ={{pt:6, pl: 10}}>
                        <TextField id={keys[10]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + nmv.length.toString()} value={valueStates[keys[10]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={keys[11]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + nmv.length.toString()} value={valueStates[keys[11]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={keys[12]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + ncv.length.toString()} value={valueStates[keys[12]]} onChange={handleTextField} required/>
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
                        <TextField id={keys[13]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + nmv.length.toString()} value={valueStates[keys[13]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={keys[14]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + nmv.length.toString()} value={valueStates[keys[14]]} onChange={handleTextField} required/>
                        <Box />
                        <TextField id={keys[15]} variant="outlined" helperText={DATA_TYPES[1]+", length: " + ncv.length.toString()} value={valueStates[keys[15]]} onChange={handleTextField} required/>
                    </Box>
                </Box>
            </Box>

            <Box sx={{width: "40%", pt: 2}}>
                <Box sx={{pt: 2, height: "30%", display: "flex", flexDirection: "row" }} >
                    
                    <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                        {variableRender(ncv.length, "CV")}
                    </Box>
                    <Box sx={{width: "25%"}}> 
                        {ncv.map((course, index) => {
                            return (
                            <Box key={index} sx={{width: "80%", pt: 3}}> 
                                <Typography variant="h5" key={index}> {course} </Typography>
                            </Box>
                            )
                        })}
                    </Box>
                </Box>
                <Box sx={{pt: 2, height: "30%", display: "flex", flexDirection: "row" }} >
                    
                    <Box sx={{width: "25%", pt: 2, display: "flex", flexDirection: "row"}}> 
                        {variableRender(nmv.length, "MV")}
                    </Box>
                    <Box sx={{width: "25%"}}> 
                        {nmv.map((course, index) => {
                            return (
                            <Box key={index} sx={{width: "80%", pt: 3}}> 
                                <Typography variant="h5" key={index}> {course} </Typography>
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

