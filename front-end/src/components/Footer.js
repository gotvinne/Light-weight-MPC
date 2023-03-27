import React from "react";
import { Box, Typography } from "@mui/material";
import CopyrightIcon from '@mui/icons-material/Copyright';

import "../css/Footer.css"

export default function Footer() {
    return (
        <div className="Footer">
            <Box sx={{pt: 2, pb: 2, display: "flex", flexDirection: "row"}}>
                <CopyrightIcon sx={{color:"#ff1344", pl: "40%"}} /> 
                <Box sx={{pl: 0.5}} />
                <Typography color="#ff1344" sx={{pt: 0.2, fontWeight: 'bold'}} align="center">
                    2023. Copyright Geir Ola Tvinnereim
                </Typography>
            </Box>
        </div>
    );
}
