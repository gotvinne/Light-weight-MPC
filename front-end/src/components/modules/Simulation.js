import React, { useState, useEffect } from "react";
import { Box } from "@mui/material";

//import backend from "../../webassembly.mjs";

import "../../css/Modules.css"

const path = "../../../back-end/data/systems";

export default function Simulation() {

    const [text, setText] = useState();
    
    useEffect(() => { // Called for every rerender. 
        setText(path);
    }, []);
    //let wasm: any;

    //backend()
      //  .then((module) => {
      //      wasm = module
      //      setText(wasm.sayHello(path));
      //      })
      //  .catch(setText)

    return (    
    <div className="Simulation">
        <Box sx={{pt: 2, pl: 2 }}>
            <div> {text} </div>
        </Box>
    </div>
    );
}