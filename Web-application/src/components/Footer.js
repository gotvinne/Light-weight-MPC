import React from "react";
import { Box, Typography } from "@mui/material";
import CopyrightIcon from '@mui/icons-material/Copyright';

import "../css/App.css"

/**
 * Defining the App footer
 */
export default function Footer() {
    return (
        <div className="Footer">
            <Box sx={{pt: 2, pb: 2, display: "flex", flexDirection: "row"}}>
                <CopyrightIcon sx={{color:"#ec4335", pl: "43%"}} /> 
                <Box sx={{pl: 0.5}} />
                <Typography color="#ec4335" sx={{pt: 0.2, pl: 1, fontWeight: 'bold'}} align="center">
                    BSD 3-Clause License
                </Typography>
            </Box>
        </div>
    );
}
