import React, { useEffect } from "react";
import { Box, Typography } from "@mui/material";

import "../../css/Modules.css"

/**
 * Describe the simulation module for plotting data
 * @param {React.useState} sim_data MPC simulation data  
 */
export default function Simulation({sim_data}) {

    return (    
    <div className="Simulation">

        <Box sx={{width: "inherit", pt: 3}}>
            <Box sx={{pl: "2%"}}>
                <Typography> CV plots </Typography>
                <div> {sim_data} </div>
            </Box>
            <Box sx={{pl: "2%"}}>
                <Typography> MV plots </Typography>
            </Box>
        </Box>
    </div>
    );
}