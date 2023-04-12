import React, { useState } from "react";
import { Box } from "@mui/material";

import LightWeightMPC from "./../web.mjs";

import "../css/Simulation.css"

const path = "../../../back-end/data/systems";
//const str = "SingleWell"; 

export default function Simulation() {

    const [text, setText] = useState();
    let wasm: any;

    LightWeightMPC().then((module) => {
        wasm = module
        setText(wasm.sayHello(path));
    });

    return (    
    <div className="Simulation">
        <Box sx={{pt: 2, pl: 2 }}>
            <p>Let's do some basic addition:</p>  
            <p> testChar </p>
            <div> {text} </div>
        </Box>
    </div>
    );
}