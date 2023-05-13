import React from "react";
import { Box, Typography } from "@mui/material";

import "../css/App.css"

/**
 * Defining the frontend header
 */
export default function Header() {
    return (
        <div className="Header">
            <Box sx={{width: "inherit", pt: 1, pb: 1, display: "flex", flexDirection: "row"}}>
                <Box sx={{pl: "40%", width: "20%"}} >
                    <Typography variant="h4" color="#ec4335" sx={{pt: "5%", fontWeight: 'bold'}}>
                        Light-weight MPC
                    </Typography>
                </Box>
                <Box sx={{pl: "35%"}}>
                    <img src={require("./../img/small_logo.png")} alt="small_logo" width="45%"/>
                </Box>
            </Box>
        </div>
    );
}
