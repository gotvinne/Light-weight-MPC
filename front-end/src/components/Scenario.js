import React, { useState, useEffect } from "react";
import { TextField, Box, Typography, Button, MenuItem, FormControl, InputLabel } from "@mui/material";
import Select, { SelectChangeEvent } from '@mui/material/Select';

import 'katex/dist/katex.min.css';
import { BlockMath } from 'react-katex';
import { importSystems } from "../utils/json-parse.js";

import LightWeightMPC from "./../web.mjs";

const DataTypes = ["double", "vector<double>", "string", "int"];
const Formulas = [`\\leq \\Delta U \\leq`, `\\leq U \\leq`, `\\leq Y \\leq`];
const TextFields = { "System": "", "Scenario": "", "T": 0, "P": 0, "M": 0, "W": 0, "Q": "[ ]", "R": "[ ]", "RoH": "[ ]", "RoL": "[ ]", "ldu": "[ ]", "lu": "[ ]", "ly": "[ ]", "udu": "[ ]", "uu": "[ ]", "uy": "[ ]"};

export default function Scenario() { // Everything is rendered inside this function
    const keys = Object.keys(TextFields); // Access keys
    
    //** HOOKS */
    const [valueStates, setValueStates] = useState(TextFields); // Initialize ValueStates is a dictionary of hooks
    const [json, setJson] = useState();
    const [systems, setSystems] = useState([]);

    useEffect(() => { // Have no ide why this runs twice!
        importSystems(systems, setSystems);
    }, [systems]);
    
    //** HANDLER FUNCTIONS */ 
    const handleSimulatonClick = e => {
        let wasm: any;
        LightWeightMPC().then((module) => {
            wasm = module
            setJson(wasm.sayHello(JSON.stringify(valueStates)));
        });
        console.log(valueStates);
    };

    const handleTextField = e => { // Update react hook
        setValueStates(TextFields => ({...TextFields, [e.target.id]: e.target.value})) // Update dictionary hook
    }

    const handleSelect = (event: SelectChangeEvent) => {
        setValueStates(TextFields => ({...TextFields, [keys[0]]: event.target.value}))
    };

    return (
        <div className="Scenario">
        <Box sx={{ width: "inherit", pt: 3, pl: "2%", flexDirection: "row", display: "flex"}}>
            <Box sx={{width: "60%"}}>

            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <FormControl style={{minWidth: "20%"}}>
                    <InputLabel id={keys[0]}> System name </InputLabel>
                    <Select
                        id={keys[0]}
                        value={valueStates[keys[0]]}
                        label="System name"
                        onChange={handleSelect} 
                    >   
                    {systems.map((course, index) => {
                        return (
                        <MenuItem value={course} key={index}> {course} </MenuItem>
                        )
                    })}
                    </Select>
                </FormControl>
            
                <TextField id={keys[1]} variant="outlined" helperText={"Scenario name, " + DataTypes[2]} value={valueStates[keys[1]]} onChange={handleTextField} required/>
                <Box sx={{pl: 2}}/>
                <TextField id={keys[2]} variant="outlined" helperText={"MPC horizon T, " + DataTypes[3]} value={valueStates[keys[2]]} onChange={handleTextField} required/>
                <Box sx={{pl: "2%"}}/>
                <Button variant="contained" size="large" color="success" onClick={handleSimulatonClick}> RUN SIMULATION </Button>
            </Box>
            <Box align="left" sx={{pl: 30, pt: 2}}>
                <Typography variant="h5" sx={{fontWeight: 'bold'}}> Model Predictive Controller: </Typography>
            </Box>
           
            <Box sx={{pl: 6, pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:2, pl: 10}}>
                    <TextField id={keys[3]} variant="outlined" helperText={"Prediction horizon P, " + DataTypes[0]} value={valueStates[keys[3]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[4]} variant="outlined" helperText={"Control horizon M, " + DataTypes[0]} value={valueStates[keys[4]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[5]} variant="outlined" helperText={"Time delay, W " + DataTypes[0]} value={valueStates[keys[5]]} onChange={handleTextField} required/>
                </Box>

                <Box sx={{pt:2, pl: 5}}>
                    <TextField id={keys[6]} variant="outlined" helperText={"Q, " + DataTypes[1]} value={valueStates[keys[6]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[7]} variant="outlined" helperText={"R, " +DataTypes[1]} value={valueStates[keys[7]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[8]} variant="outlined" helperText={"RoH, " +DataTypes[0]} value={valueStates[keys[8]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[9]} variant="outlined" helperText={"RoL, " +DataTypes[0]} value={valueStates[keys[9]]} onChange={handleTextField} required/>
                </Box>
            </Box>

            <Box sx={{pt: 2, display: "flex", flexDirection: "row"}}>
                <Box sx ={{pt:6, pl: 10}}>
                    <TextField id={keys[10]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[10]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[11]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[11]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[12]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[12]]} onChange={handleTextField} required/>
                </Box>

                <Box >
                <Typography variant="h5" align="center" sx={{fontWeight: 'bold'}}> Constraints: </Typography>
                <div className="katex"> 
                    <Box sx={{pt: 1}}/>
                    <BlockMath math={Formulas[0]} />
                    <Box sx={{pt: 1}}/>
                    <BlockMath math={Formulas[1]} />
                    <Box sx={{pt: 1}}/>
                    <BlockMath math={Formulas[2]} />
                </div>
                </Box >
                    
                <Box sx ={{pt: 6}}>
                    <TextField id={keys[13]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[13]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[14]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[14]]} onChange={handleTextField} required/>
                    <Box />
                    <TextField id={keys[15]} variant="outlined" helperText={DataTypes[1]} value={valueStates[keys[15]]} onChange={handleTextField} required/>
                </Box>
            </Box>
            </Box>

            <Box sx={{width: "40%", pt: 2, display: "flex", flexDirection: "row"}}>
                <Typography> {json} </Typography>
            </Box>
        </Box>
        </div>
    );
}

