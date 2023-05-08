import React, { useMemo } from "react";
import { Box, Typography } from '@mui/material';
import { PlotPrediction, PlotActuation } from "../../utils/plot.js";

const PLOTS_PER_ROW = 2;

export function OutputCharts({CVs, T, P, refData}) {

    const [cols, rows, odd_plot] = useMemo(() => {
        let num_rows = Math.floor((CVs.length + 1) / PLOTS_PER_ROW);
        let odd_plot = Boolean(CVs.length % 2);

        return [[...Array(PLOTS_PER_ROW).keys()], [...Array(num_rows).keys()], odd_plot];   
    }, []);

    return (
        <div className="Scenario">
            {rows.map((i) => {
                return (
                <Box key={i} sx={{pl:"10%", width: "45%", display: "flex", flexDirection: "row"}}>
                    {cols.map((j) => {
                        return (
                            <div>
                                {(odd_plot && (i+1) === (rows.length) && j === 1) 
                                ?  <Box key={"unique"} /> 
                                : <Box key={j}> 
                                        {PlotPrediction(CVs[j * (i+1)], T, P, refData[j * (i+1)])}
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

export function ActuationCharts({MVs, T, P}) {
    
    const [cols, rows, odd_plot] = useMemo(() => {
        let num_rows = Math.floor((MVs.length + 1) / PLOTS_PER_ROW);
        let odd_plot = Boolean(MVs.length % 2);

        return [[...Array(PLOTS_PER_ROW).keys()], [...Array(num_rows).keys()], odd_plot];   
    }, []);

    return (
        <div className="Scenario">
            {rows.map((i) => {
                return (
                <Box key={i} sx={{pl:"10%", width: "45%", display: "flex", flexDirection: "row"}}>
                    {cols.map((j) => {
                        return (
                            <div>
                                {(odd_plot && (i+1) === (rows.length) && j === 1) 
                                ?  <Box key={"unique"} /> 
                                : <Box key={j}> 
                                        {PlotActuation(MVs[j * (i+1)], T, P)}
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

