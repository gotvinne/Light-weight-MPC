import React from "react";
import { Box, Typography } from "@mui/material";

import "../css/App.css"

/**
 * Defining the frontend header
 */
export default function Header() {
    return (
        <div className="Header">
            <Box sx={{width: "inherit", pt: 2, pb: 2, display: "flex", flexDirection: "row"}}>
                <Box sx={{pl: "40%", width: "20%"}} >
                    <Typography variant="h4" color="#ff1344" sx={{pt: "3%", fontWeight: 'bold'}}>
                        Light-weight MPC
                    </Typography>
                </Box>
                <Box sx={{pl: "30%"}}>
                    <img src={require("./../img/small_logo.png")} width="40%"/>
                </Box>
            </Box>
        </div>
    );
}
