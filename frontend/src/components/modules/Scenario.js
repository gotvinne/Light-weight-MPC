import React, { useState, useEffect, useMemo } from "react";
import { TextField, Box, Button, MenuItem, FormControl, InputLabel, Select } from "@mui/material";
import { importSystems, readModelParams, readSystem, serializeRef, serializeScenario } from "../../utils/IO.js";
import { onlyNumbers, updateError } from "../../utils/error.js";
import { simulate } from "../../utils/wasm.js";

import "../../css/Modules.css"
import Reference from "./textfields/Reference.js";
import Constraints from "./textfields/Constraints.js";
import Tuning from "./textfields/Tuning.js";

const SCENARIO_STORAGE_KEY = 'scenario.storage';
 
const TEXT_FIELDS = { "System": "", "Scenario": "test", "T": "180", "P": "100", "M": "50", "W": "0", "Q": "[1, 100]", "R": "[1, 100]", "RoH": "[1, 1]", "RoL": "[1, 1]", "ldu": "[-2, -10]", "lu": "[0, 0]", "ly": "[0, 0]", "udu": "[2, 10]", "uu": "[100, 1000]", "uy": "[4000, 100]"};
//const TEXT_FIELDS = { "System": "", "Scenario": "test", "T": 100, "P": 10, "M": 5, "W": 0, "Q": "[1]", "R": "[1]", "RoH": "[1]", "RoL": "[1]", "ldu": "[-10]", "lu": "[-0.01]", "ly": "[-0.01]", "udu": "[10]", "uu": "[0.8]", "uy": "[1.6]"};
//const TEXT_FIELDS = { "System": "", "Scenario": "", "T": 0, "P": 0, "M": 0, "W": 0, "Q": "[]", "R": "[]", "RoH": "[]", "RoL": "[]", "ldu": "[]", "lu": "[]", "ly": "[]", "udu": "[]", "uu": "[]", "uy": "[]"};
const ERROR = {"T": false, "P": false, "M": false, "W": false, "Q": false, "R": false, "RoH": false, "RoL": false, "ldu": false, "lu": false, "ly": false, "udu": false, "uu": false, "uy": false};
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
    const [ref, setRef] = useState("");
    const [buttonDisable, setButtonDisable] = useState(true);
        
    const [cv, mv] = useMemo(() => { // Read model params for displayment
        if (sce[KEYS[0]] === "") {
            return [[[], []], [[], []]];
        } else {
            let cv_data = readModelParams(sce[KEYS[0]], "CV");
            setRef(() => {return Array(cv_data[0].length).fill("0")});
            return [cv_data, readModelParams(sce[KEYS[0]], "MV")];
        }
    }, [sce[KEYS[0]]]);

    //** USE EFFECTS */
    useEffect(() => { // Fetch storage
        const storedSce = JSON.parse(localStorage.getItem(SCENARIO_STORAGE_KEY));
        if (storedSce !== null) { setSce(storedSce); }
    }, []);

    useEffect(() => { // Store sce
        if (sce !== TEXT_FIELDS) {
            localStorage.setItem(SCENARIO_STORAGE_KEY, JSON.stringify(sce));
        }
        
        if (sce[KEYS[0]] !== "") {
            setError({...updateError(sce, error, cv[0].length, mv[0].length)});
            if (Object.values(error).every((v) => v === false) && sce[KEYS[0]] !== "" && onlyNumbers(ref)) {
                setButtonDisable(false);
            } else {
                setButtonDisable(true);
            }
        }
    }, [sce])
    
    //** HANDLER FUNCTIONS */ 
    const handleSimulatonClick = () => {
        // Check TextFields if valid inputs are given.
        const sys_file = readSystem(sce[KEYS[0]]);
        const sce_file = serializeScenario(sce);
        const ref_str = serializeRef(ref);

        // Might use async
        // MPC simulation:
        simulate(sce_file, sys_file, sce[KEYS[1]], ref_str, parseInt(sce[KEYS[2]]), simHook);
    };

    const handleTextField = e => { 
        setSce(sce => { return {...sce, [e.target.id]: e.target.value} }); 
    }

    const handleSelect = e => { 
        setSce(sce => { return {...sce, [KEYS[0]]: e.target.value} });
    };

    const handleReference = e => {
        let newRef = [...ref];
        newRef[parseInt(e.target.id)] = e.target.value;
        setRef(newRef); 
    }

    return (
        <div className="Scenario">
        <Box sx={{ width: "inherit", pt: 3, flexDirection: "row", display: "flex"}}>
           
            <Box sx={{width: "50%", pl: "2%"}}> 
                <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                    <FormControl style={{minWidth: "20%"}}>
                        <InputLabel id={KEYS[0]}> System name </InputLabel>
                        <Select id={KEYS[0]} value={sce[KEYS[0]]} label="System name" onChange={handleSelect} >   
                        {systemNames.map((course, index) => {
                            return (
                            <MenuItem value={course} key={index}> {course} </MenuItem>
                            )
                        })}
                        </Select>
                    </FormControl>
                
                    <TextField id={KEYS[1]} variant="outlined" helperText={"Scenario name, string"} value={sce[KEYS[1]]} onChange={handleTextField} required/>
                    <Box sx={{pl: 2}}/>
                    <TextField error={error[KEYS[2]]} sx={{width: "20%"}} id={KEYS[2]} variant="outlined" helperText={"MPC horizon T, int"} value={sce[KEYS[2]]} onChange={handleTextField} required/>
                    <Box sx={{pl: "3%"}}/>
                    <Button variant="contained" size="large" color={"success"} disabled={buttonDisable} onClick={handleSimulatonClick}> RUN SIMULATION </Button>
                </Box>

                <Tuning keys={KEYS} ncv={cv[0].length} nmv={mv[0].length} error={error} scenario={sce} handler={handleTextField} />
                <Constraints keys={KEYS} nmv={mv[0].length} ncv={cv[0].length} error={error} scenario={sce} handler={handleTextField} />
            
            </Box>

            <Reference cv={cv} mv={mv} value={ref} handler={handleReference}/>

        </Box>
        </div>
    );
}
