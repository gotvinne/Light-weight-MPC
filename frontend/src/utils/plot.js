
import React from "react";
import Plot from "react-plotly.js"

const BLACK = 'rgb(0, 0, 0)';
const BLUE = 'rgb(0, 0, 255)';
const RED = 'rgb(255, 0, 0)';

export function PlotPrediction(cv_data, T, P, ref) {
    let t = Array.from({ length: T }, (value, index) => index);
    let lower = Array(T).fill(cv_data.c[0]);
    let upper = Array(T).fill(cv_data.c[1]);
    let refData = Array(T).fill(ref);

    var prediction = {
        x: t.slice(0, T-P),
        y: cv_data.y_pred.slice(0, T-P),
        name: "Output",
        type: "line",
        line: {
            color: 'rgb(128, 0, 128)'
        }
    };

    var ref = {
        x: t,
        y: refData,
        name: "Reference",
        type: "line",
        line: {
            color: RED,
            dash: "dashdot"
        }
    };

    var prediction_pred = {
        x: t.slice(T-P, t.lenght),
        y: cv_data.y_pred.slice(T-P, t.length),
        name: "Pred Output",
        type: "line",
        line: {
            color: 'rgb(128, 0, 128)',
            dash: "dot"
        }
    };

    var lower_constraint = {
        x: t, 
        y: lower,
        mode: 'lines',
        name: "Lower limit",
        line: {
            dash: 'dashdot',
            width: 2,
            color: BLACK
        }
    } 

    var upper_constraint = {
        x: t, 
        y: upper,
        mode: 'lines',
        name: "Upper limit",
        line: {
            dash: 'dashdot',
            width: 2,
            color: BLACK
        }
    } 

    var layout = {
        title: cv_data.output,
        xaxis: {
            title: "MPC horizon, t"
        },
        yaxis: {
            title: cv_data.unit
        },
        shapes: [{ //line vertical
            yref: "paper",
            type: 'line',
            x0: T-P,
            y0: 0,
            x1: T-P,
            y1: 1,
            line: { 
                color: BLACK,
                width: 1
            }
        }]
    };

    return (
        <Plot data={[prediction, prediction_pred, ref, lower_constraint, upper_constraint]} layout={layout}/>
    ); 
}

export function PlotActuation(mv_data, T, P) {
    let t = Array.from({ length: T }, (value, index) => index);
    let lower = Array(T).fill(mv_data.c[0]);
    let upper = Array(T).fill(mv_data.c[1]);

    var actuation = {
        x: t.slice(0, T-P),
        y: mv_data.u.slice(0, T-P),
        name: "Actuation",
        type: 'scatter',
        line: {
            color: BLUE,
            shape: 'hv'
        }
    };

    var actuation_pred = {
        x: t.slice(T-P, t.length),
        y: mv_data.u.slice(T-P, t.length),
        name: "Pred actuation",
        type: "scatter",
        line: {
            color: BLUE,
            dash: "dot",
            shape: 'hv'
        }
    };

    var lower_constraint = {
        x: t, 
        y: lower,
        mode: 'lines',
        name: "Lower limit",
        line: {
            dash: 'dashdot',
            width: 2,
            color: BLACK
        }
    } 

    var upper_constraint = {
        x: t, 
        y: upper,
        mode: 'lines',
        name: "Upper limit",
        line: {
            dash: 'dashdot',
            width: 2,
            color: BLACK
        }
    } 

    var layout = {
        title: mv_data.input,
        xaxis: {
            title: "MPC horizon, t"
        },
        yaxis: {
            title: mv_data.unit
        },
        shapes: [{ //line vertical
            yref: "paper",
            type: 'line',
            x0: T-P,
            y0: 0,
            x1: T-P,
            y1: 1,
            line: { 
                color: BLACK,
                width: 1
            }
        }]
    };

    return (
        <Plot data={[actuation, actuation_pred, lower_constraint, upper_constraint]} layout={layout}/>
    ); 
}
