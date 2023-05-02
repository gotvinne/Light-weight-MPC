
import React from "react";
import Plot from "react-plotly.js"

export function PlotPrediction(cv_data, T) {
    let t = Array.from({ length: T }, (value, index) => index);

    var data = [
        {
            x: t,
            y: cv_data.y_pred,
            type: "line",
            line: {
                color: 'rgb(128, 0, 128)'
            }
        }
    ];

    var layout = {
        title: cv_data.output,
        xaxis: {
            title: "MPC horizon, t"
        },
        yaxis: {
            title: cv_data.unit
        }
    };

    return (
        <Plot data={data} layout={layout}/>
    ); 
}
