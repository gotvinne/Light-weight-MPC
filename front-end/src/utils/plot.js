
import React from "react";
import Plot from "react-plotly.js"

export function PlotPrediction(cv_data, T) {
    let t = Array.from({ length: T }, (value, index) => index);
    let lower = Array(T).fill(cv_data.c[0]);
    let upper = Array(T).fill(cv_data.c[1]);

    var prediction = {
        x: t,
        y: cv_data.y_pred,
        name: "Prediction",
        type: "line",
        line: {
            color: 'rgb(128, 0, 128)'
        }
    };

    var lower_constraint = {
        x: t, 
        y: lower,
        mode: 'lines',
        name: "Lower",
        line: {
            dash: 'dot',
            width: 2,
            color: 'rgb(0, 0, 0)'
        }
    } 

    var upper_constraint = {
        x: t, 
        y: upper,
        mode: 'lines',
        name: "Upper",
        line: {
            dash: 'dot',
            width: 2,
            color: 'rgb(0, 0, 0)'
        }
    } 

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
        <Plot data={[prediction, lower_constraint, upper_constraint]} layout={layout}/>
    ); 
}

export function PlotActuation(mv_data, T) {
    let t = Array.from({ length: T }, (value, index) => index);
    let lower = Array(T).fill(mv_data.c[0]);
    let upper = Array(T).fill(mv_data.c[1]);

    var actuation = {
        x: t,
        y: mv_data.u,
        name: "Actuation",
        type: "line",
        line: {
            color: 'rgb(0, 0, 255)'
        }
    };

    var lower_constraint = {
        x: t, 
        y: lower,
        mode: 'lines',
        name: "Lower",
        line: {
            dash: 'dot',
            width: 2,
            color: 'rgb(0, 0, 0)'
        }
    } 

    var upper_constraint = {
        x: t, 
        y: upper,
        mode: 'lines',
        name: "Upper",
        line: {
            dash: 'dot',
            width: 2,
            color: 'rgb(0, 0, 0)'
        }
    } 

    var layout = {
        title: mv_data.input,
        xaxis: {
            title: "MPC horizon, t"
        },
        yaxis: {
            title: mv_data.unit
        }
    };

    return (
        <Plot data={[actuation, lower_constraint, upper_constraint]} layout={layout}/>
    ); 
}
