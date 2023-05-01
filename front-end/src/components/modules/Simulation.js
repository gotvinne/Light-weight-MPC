import React, { useState, useEffect } from "react";
import { Box, Typography } from "@mui/material";

import { readSimParams, readSimCV, readSimMV, SimParam, MV, CV } from "../../utils/IO.js";
import "../../css/Modules.css"

/**
 * Describe the simulation module for plotting data
 * @param {React.useState} sim_data MPC simulation data  
 */
export default function Simulation({sim_data}) {

    const [simAvaliable, setSimAvaliable] = useState(false);
    const [simParam, setSimParam] = useState();
    const [CVs, setCVs] = useState();
    const [MVs, setMVs] = useState();

    useEffect(() => {
        if (sim_data === "") {
            setSimAvaliable(false);
            return;
        } else {
            const json_sim = JSON.parse(sim_data);
            setSimParam(readSimParams(json_sim));
            setCVs(readSimCV(json_sim));
            setMVs(readSimMV(json_sim));
            setSimAvaliable(true);
        }
    }, [sim_data]);

    return (    
    <div className="Simulation">
        {simAvaliable
            ? <Box sx={{width: "inherit"}}> 
                <Box sx={{width: "inherit", pt: 3, height: "30%", display: "flex", flexDirection: "row"}}>
                    {CVs.map((cv, index) => {
                        return (
                        <Box key={index} sx={{width: "50%"}}> 
                            <Typography variant="h5" key={index}> {cv.output + ":"} </Typography>
                        </Box>
                        )
                    })}
                </Box>

                <Box sx={{width: "inherit", pt: 3, height: "30%", display: "flex", flexDirection: "row"}}>
                {MVs.map((mv, index) => {
                    return (
                    <Box key={index} sx={{width: "50%"}}> 
                        <Typography variant="h5" key={index}> {mv.input + ":"} </Typography>
                    </Box>
                    )
                })}
                </Box>
            </Box>

            : <Box sx={{width: "inherit", pt: 3}}>
                <Box sx={{pl: "2%"}}>
                    <Typography variant="h5"> No simulation avaliable! </Typography>
                </Box>
            </Box>
        }
    </div>
    );
}