import React, { useState, useEffect } from "react";
import { Box, Typography } from "@mui/material";

import { readSimParams, readSimCV, readSimMV } from "../../utils/IO.js";
import { PlotPrediction, PlotActuation } from "../../utils/plot.js";
import "../../css/Modules.css"

const initSimParam = {scenario: "", T: 0, nCV: 0, nMV: 0};

/**
 * Describe the simulation module for plotting data
 * @param {React.useState} sim_data MPC simulation data  
 */
export default function Simulation({sim_data}) {

    const [simAvaliable, setSimAvaliable] = useState(false);
    const [simParam, setSimParam] = useState(initSimParam);
    const [CVs, setCVs] = useState([]);
    const [MVs, setMVs] = useState([]);

    useEffect(() => {
        if (sim_data === "") {
            setSimAvaliable(false);
            return;
        } else {
            const json_sim = JSON.parse(sim_data);
            setSimParam(readSimParams(json_sim));
            setCVs(readSimCV(json_sim));
            setMVs(readSimMV(json_sim));
            
        }
    }, [sim_data]);

    useEffect(() => {
        if (simParam !== initSimParam) {
            setSimAvaliable(true);
        }
    }, [simParam])

    return (    
    <div className="Simulation">
        {simAvaliable
            ? <Box sx={{width: "inherit"}}> 
                <Box sx={{width: "inherit", pl:"10%", height: "30%", display: "flex", flexDirection: "row"}}>
                    {CVs.map((cv, index) => {
                        return (
                        <Box key={index} > 
                            {PlotPrediction(cv, simParam.T)}
                        </Box>
                        )
                    })}
                </Box>

                <Box sx={{width: "inherit", pl:"10%", height: "30%", display: "flex", flexDirection: "row"}}>
                    {MVs.map((mv, index) => {
                        return (
                        <Box key={index} > 
                            {PlotActuation(mv, simParam.T)}
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