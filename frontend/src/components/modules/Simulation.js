import React, { useState, useEffect } from "react";
import { Box, Typography } from "@mui/material";

import { readSimParams, readSimCV, readSimMV } from "../../utils/IO.js";
import { OutputCharts, ActuationCharts } from "./LineChart.js";
import "../../css/Modules.css"

const initSimParam = {scenario: "", T: 0, nCV: 0, nMV: 0};

/**
 * Describe the simulation module for plotting data
 * @param {React.useState} simData MPC simulation data  
 */
export default function Simulation({simData}) {

    const [simAvaliable, setSimAvaliable] = useState(false);
    const [simParam, setSimParam] = useState(initSimParam);
    const [CVs, setCVs] = useState([]);
    const [MVs, setMVs] = useState([]);

    useEffect(() => {
        if (simData === "") {
            setSimAvaliable(false);
        } else {
            console.log(simData)
            const json_sim = JSON.parse(simData);
            setSimParam(readSimParams(json_sim));
            setCVs(readSimCV(json_sim));
            setMVs(readSimMV(json_sim));
        }
    }, [simData]);

    useEffect(() => {
        if (simParam !== initSimParam) {
            setSimAvaliable(true);
        }
    }, [simParam])

    return (    
    <div className="Simulation">
        {simAvaliable
            ?   <Box sx={{width: "inherit"}}> 
                    <Box sx={{pl: "2%", pt: "1%"}}>
                        <Typography variant="h5"> {"Scenario: " + simParam.scenario} </Typography>
                    </Box>
                    <OutputCharts CVs={CVs} T={simParam.T} P={simParam.P}/>
                    <ActuationCharts MVs={MVs} T={simParam.T} />
                </Box>

            : <Box sx={{width: "inherit", pt: 3}}>
                <Box sx={{pl: "2%"}}>
                    <Typography variant="h5"> Scenario: No simulation avaliable! </Typography>
                </Box>
            </Box>
        }
    </div>
    );
}