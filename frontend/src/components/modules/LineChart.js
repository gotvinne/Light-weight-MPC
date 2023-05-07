import React from "react";
import { Box } from '@mui/material';
import { PlotPrediction, PlotActuation } from "../../utils/plot.js";

const PLOTS_PER_ROW = 2;

export function OutputCharts({CVs, T, P, refData}) {
    
    const cols = Array.from(Array(PLOTS_PER_ROW).keys());
    const rows = Array.from(Array((CVs.length + 1) % PLOTS_PER_ROW).keys());

    return (
        <div className="Scenario">
            {rows.map((i) => {
                return (
                    <Box key={i} sx={{pl:"10%", width: "45%", display: "flex", flexDirection: "row"}}>
                        {cols.map((j) => {
                            return (
                                <Box key={j}> 
                                    {PlotPrediction(CVs[j * (i+1)], T, P, refData[j])}
                                </Box>
                            );
                        })}
                    </Box>
                ); 
            })}
        </div>
    );
}

export function ActuationCharts({MVs, T, P}) {
    
    const cols = Array.from(Array(PLOTS_PER_ROW).keys());
    const rows = Array.from(Array((MVs.length + 1) % PLOTS_PER_ROW).keys());

    return (
        <div className="Scenario">
            {rows.map((i) => {
                return (
                    <Box key={i} sx={{pl:"10%", width: "45%", display: "flex", flexDirection: "row"}}>
                        {cols.map((j) => {
                            return (
                                <Box key={j}> 
                                    {PlotActuation(MVs[j * (i+1)], T, P)}
                                </Box>
                            );
                        })}
                    </Box>
                ); 
            })}
        </div>
    );
}

