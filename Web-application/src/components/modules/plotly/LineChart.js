import React, { useMemo } from "react";
import { Box } from '@mui/material';
import { PlotPrediction, PlotActuation } from "./plot.js";

const PLOTS_PER_ROW = 2;

export function OutputCharts({CVs, T, P}) {

    const [cols, rows, odd_plot] = useMemo(() => {
        let num_rows = Math.floor((CVs.length + 1) / PLOTS_PER_ROW);
        let odd_plot = Boolean(CVs.length % 2);

        return [[...Array(PLOTS_PER_ROW).keys()], [...Array(num_rows).keys()], odd_plot];   
    }, [CVs]);
    // Keys are set randomly to avoid warning
    return (
        <div>
            {rows.map((i) => {
                return (
                <Box key={i} sx={{pl:"1%", width: "45%", display: "flex", flexDirection: "row"}}>
                    {cols.map((j) => {
                        return (
                            <div key={10+j}>
                                {(odd_plot && (i+1) === (rows.length) && j === 1) 
                                ?  <Box key={j+i} /> 
                                : <Box key={j+i}> 
                                        {PlotPrediction(CVs[j * (i+1)], T, P)}
                                    </Box>
                                }
                            </div>
                        );
                    })}
                </Box>
                );
            })}
        </div>
    );
}

export function ActuationCharts({MVs, T, M}) {
    
    const [cols, rows, odd_plot] = useMemo(() => {
        let num_rows = Math.floor((MVs.length + 1) / PLOTS_PER_ROW);
        let odd_plot = Boolean(MVs.length % 2);

        return [[...Array(PLOTS_PER_ROW).keys()], [...Array(num_rows).keys()], odd_plot];   
    }, [MVs]);
    // Keys are set randomly to avoid warning
    return (
        <div>
            {rows.map((i) => {
                return (
                <Box key={i} sx={{pl:"1%", width: "45%", display: "flex", flexDirection: "row"}}>
                    {cols.map((j) => {
                        return (
                            <div key={10+j}>
                                {(odd_plot && (i+1) === (rows.length) && j === 1) 
                                ?  <Box key={j+i} /> 
                                : <Box key={j+i}> 
                                        {PlotActuation(MVs[j * (i+1)], T, M)}
                                    </Box>
                                }
                            </div>
                        );
                    })}
                </Box>
                );
            })}
        </div>
    );
}

