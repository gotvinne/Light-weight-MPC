import React, { useState, useEffect } from "react";
import { Box } from "@mui/material";

import LightWeightMPC from "./../web.mjs";

import "../css/Simulation.css"

const str = "Hei!";

export default function Simulation() {
    const [add, setAdd] = useState();
    const [testChar, setTestChar] = useState(); // Declare state variable
    useEffect( () => {LightWeightMPC().then((Module) => { 
    setAdd(() => Module.cwrap("add", "number", ["number", "number"]));
    setTestChar(() => Module.cwrap("testChar", "string", ["string"])); 
    }); }, []);  

    if (!add) {  
        return "Loading webassembly...";  
    }  return (    
    <div className="Simulation">
        <Box sx={{ width: "100%", pt: 2, pl: 2 }}>
            <p>Let's do some basic addition:</p>  
            <div> {add(1,89)} </div> 
            <p> testChar </p>
            <div> {testChar(str)} </div>
        </Box>
    </div>
    );
}