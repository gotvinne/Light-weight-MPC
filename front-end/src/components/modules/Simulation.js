import React from "react";
import { Box } from "@mui/material";

import "../../css/Modules.css"

/**
 * 
 * @param {React.useState} sim_data MPC simulation data  
 * @returns 
 */
export default function Simulation({sim_data}) {

    return (    
    <div className="Simulation">
        <Box sx={{pt: 2, pl: 2 }}>
            <div> {sim_data} </div>
        </Box>
    </div>
    );
}