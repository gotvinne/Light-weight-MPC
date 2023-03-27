import React, { useState, useEffect } from "react";
import { Box } from "@mui/material";

import LightWeightMPC from "./../web.mjs";

import "../css/Simulation.css"

export default function Simulation() {
    const [add, setAdd] = useState();
    useEffect( () => {LightWeightMPC().then((Module) => { 
    setAdd(() => Module.cwrap("add", "number", ["number", "number"]));
    }); }, []);  

    if (!add) {  
        return "Loading webassembly...";  
    }  return (    
    <div className="Simulation">
        <Box sx={{ width: "100%", pt: 2, pl: 2 }}>
            <p>Let's do some basic addition:</p>  
            <div> {add(1,89)} </div> 
        </Box>
    </div>
    );
}