import React, { useState, useEffect, useMemo } from "react";
import { TextField, Box, Button, MenuItem, FormControl, InputLabel, Select, Typography } from "@mui/material";
import { importSystems, readModelParams, readSystem, serializeRef, serializeScenario } from "../../utils/IO.js";
import { onlyNumbers, updateError } from "../../utils/error.js";
import { simulate } from "../../utils/wasm.js";

import "../../css/Modules.css"
import Reference from "./textfields/Reference.js";
import Constraints from "./textfields/Constraints.js";
import Tuning from "./textfields/Tuning.js";

const SCENARIO_STORAGE_KEY = 'scenario.storage';

// Initial states
const TEXT_FIELDS = { "System": "", "Scenario": "", "T": "0", "P": "0", "M": "0", "W": "0", "Q": "[]", "R": "[]", "RoH": "[]", "RoL": "[]", "ldu": "[]", "lu": "[]", "ly": "[]", "udu": "[]", "uu": "[]", "uy": "[]"};
const ERROR = {"T": false, "P": false, "M": false, "W": false, "Q": false, "R": false, "RoH": false, "RoL": false, "ldu": false, "lu": false, "ly": false, "udu": false, "uu": false, "uy": false};

/**
 * Module defining MPC scenario
 * @param {React.useState} simHook hook to store simulation data  
 */
export default function Scenario({simHook}) {
    //** HOOKS */
    const [sce, setSce] = useState(TEXT_FIELDS);
    const [systemNames] = useState(importSystems());
    const [error, setError] = useState(ERROR);
    const [ref, setRef] = useState("");
    const [buttonDisable, setButtonDisable] = useState(true);
    const [simStatus, setSimStatus] = useState({status: "", error: ""});
        
    const [cv, mv] = useMemo(() => { // When system is selected, c/mv[0] = data, units
        if (sce["System"] === "") {
            return [[[], []], [[], []]];
        } else {
            let cv_data = readModelParams(sce["System"], "CV");
            setRef(() => {return Array(cv_data[0].length).fill("0")});

            setSce(sce => {
                const resource = require(`./default_tuning.json`); 
                if (resource.hasOwnProperty(sce["System"])) {
                    return resource[sce["System"]];
                } else { 
                    return sce; 
                }
            })
            return [cv_data, readModelParams(sce["System"], "MV")];
        }
    }, [sce["System"]]);

    //** USE EFFECTS */
    useEffect(() => { // Fetch storage
        const storedSce = JSON.parse(localStorage.getItem(SCENARIO_STORAGE_KEY));
        if (storedSce !== null) { setSce(storedSce); }
    }, []);

    useEffect(() => { // Store sce
        if (sce !== TEXT_FIELDS) {
            localStorage.setItem(SCENARIO_STORAGE_KEY, JSON.stringify(sce));
        }
        
        if (sce["System"] !== "") { // Check is system update
            setError({...updateError(sce, error, cv[0].length, mv[0].length)});
            if (Object.values(error).every((v) => v === false) && sce["System"] !== "" && onlyNumbers(ref)) {
                setButtonDisable(false);
            } else {
                setButtonDisable(true);
            }
        }
    }, [sce, ref])
    
    //** HANDLER FUNCTIONS */ 
    const handleSimulatonClick = () => {
        const sys_file = readSystem(sce["System"]);
        const sce_file = serializeScenario(sce);
        const ref_str = serializeRef(ref);

        // MPC simulation:
        console.log("Simulating...");
        setSimStatus(simStatus => { return {...simStatus, status: "Simulating..."} });
        simHook("Simulating");
        simulate(sce_file, sys_file, sce["Scenario"], ref_str, parseInt(sce["T"]), simHook, setSimStatus);
    };

    const handleTextField = e => { 
        setSce(sce => { return {...sce, [e.target.id]: e.target.value} }); 
    }

    const handleSelect = e => { 
        setSce(sce => { return {...sce, ["System"]: e.target.value} });
    };

    const handleReference = e => {
        let newRef = [...ref];
        newRef[parseInt(e.target.id)] = e.target.value;
        setRef(newRef); 
    }

    return (
        <div className="Scenario">
        <Box sx={{ width: "inherit", pt: 3, flexDirection: "row", display: "flex"}}>
           
            <Box sx={{width: "45%", pl: "2%"}}> 
                <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                    <FormControl style={{minWidth: "27%"}}>
                        <InputLabel id={"System"}> System name </InputLabel>
                        <Select id={"System"} value={sce["System"]} label="System name" onChange={handleSelect} >   
                            {systemNames.map((course, index) => {
                                return ( <MenuItem value={course} key={index}> {course} </MenuItem> )
                            })}
                        </Select>
                    </FormControl>
                
                    <TextField id={"Scenario"} variant="outlined" helperText={"Scenario name, string"} value={sce["Scenario"]} onChange={handleTextField} required/>
                    <Box sx={{pl: 2}}/>
                    <TextField error={error["T"]} sx={{width: "30%"}} id={"T"} variant="outlined" helperText={"MPC horizon T, int"} value={sce["T"]} onChange={handleTextField} required/>
                </Box>

                <Tuning ncv={cv[0].length} nmv={mv[0].length} error={error} sce={sce} handler={handleTextField} />
                <Constraints nmv={mv[0].length} ncv={cv[0].length} error={error} sce={sce} handler={handleTextField} />
            </Box>
            
            <Box sx={{pl: "1%", pt: "1%", width: "36%"}}>
                <Reference cv={cv} mv={mv} ref_value={ref} handler={handleReference}/>

                <Box sx={{pt: 2, height: "5%", display: "flex", flexDirection: "row"}} >
                    <Typography color={"#d40808"}>
                        {simStatus.error}
                    </Typography>
                </Box>
            </Box>

            <Box textAlign="left" sx={{pl: "1%", pt: "1%", width: "25%"}}>
                <Button variant="contained" size="large" sx={{minWidth: "50%", minHeight: "12%"}} color={"success"} disabled={buttonDisable} onClick={handleSimulatonClick}> RUN SIMULATION </Button>
            </Box>
            
        </Box>
        </div>
    );
}
