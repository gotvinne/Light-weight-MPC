
import React from "react";
import Plot from "react-plotly.js"

const BLACK = 'rgb(0, 0, 0)';
const BLUE = 'rgb(0, 0, 255)';
const RED = 'rgb(255, 0, 0)';
const PURPLE = 'rgb(128, 0, 128)';

export function PlotPrediction(cv_data, T, P) {
    let t = Array.from({ length: T+P }, (value, index) => index);
    let lower = Array(T+P).fill(cv_data.c[0]);
    let upper = Array(T+P).fill(cv_data.c[1]);

    var output = {
        x: t.slice(0, T),
        y: cv_data.y_pred.slice(0, T),
        name: "Output",
        type: "line",
        line: {
            color: PURPLE
        }
    };

    var ref_plot = {
        x: t,
        y: cv_data.ref,
        name: "Reference",
        type: "line",
        line: {
            color: RED,
            width: 0.75
        }
    };

    var output_pred = {
        x: t.slice(T, t.length),
        y: cv_data.y_pred.slice(T, t.length),
        name: "Pred Output",
        mode: 'lines',
        line: {
            color: PURPLE,
            dash: "dot"
        }
    };

    var lower_constraint = {
        x: t, 
        y: lower,
        mode: 'lines',
        name: "Lower limit",
        line: {
            dash: 'dot',
            width: 1,
            color: BLACK
        }
    } 

    var upper_constraint = {
        x: t, 
        y: upper,
        mode: 'lines',
        name: "Upper limit",
        line: {
            dash: 'dot',
            width: 1,
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
            x0: T,
            y0: 0,
            x1: T,
            y1: 1,
            line: { 
                color: BLACK,
                width: 1
            }
        }]
    };

    return (
        <Plot data={[output, output_pred, ref_plot, lower_constraint, upper_constraint]} layout={layout}/>
    ); 
}

export function PlotActuation(mv_data, T, M) {
    let t = Array.from({ length: T+M }, (value, index) => index);
    let lower = Array(T+M).fill(mv_data.c[0]);
    let upper = Array(T+M).fill(mv_data.c[1]);

    var actuation = {
        x: t.slice(0, T),
        y: mv_data.u.slice(0, T),
        name: "Actuation",
        type: 'scatter',
        line: {
            color: BLUE,
            shape: 'hv'
        }
    };

    var actuation_pred = {
        x: t.slice(T, t.length),
        y: mv_data.u.slice(T, t.length),
        name: "Pred actuation",
        mode: "lines",
        line: {
            color: BLUE,
            dash: "dot"
        }
    };

    var lower_constraint = {
        x: t, 
        y: lower,
        mode: 'lines',
        name: "Lower limit",
        line: {
            dash: 'dot',
            width: 1,
            color: BLACK
        }
    } 

    var upper_constraint = {
        x: t, 
        y: upper,
        mode: 'lines',
        name: "Upper limit",
        line: {
            dash: 'dot',
            width: 1,
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
            x0: T,
            y0: 0,
            x1: T,
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
