/** MPC input error checking */
import { convertArr } from './IO.js'

/**
 * Checks if array contains only numbers or number strings
 * @param {Array} arr 
 * @returns Boolean
 */
export function onlyNumbers(arr) { 
    return arr.every(element => {
        if (!isNaN(element) && element !== "0" && element !== "") {
            return true;
        } else {
            return false;
        }
    });
}

/**
 * Updating error hook
 * @param {React.useState} sce MPC scenario hook
 * @param {React.useState} error Error hook
 * @param {Number} ncv number of controlled variables
 * @param {Number} nmv number of manipulated variables
 * @returns Updated error hook
 */
export function updateError(sce, error, ncv, nmv) {
    var curr_err = error;

    updateHorizons(sce, curr_err);
    updateTunings(sce, curr_err, ncv, nmv);
    updateConstraints(sce, curr_err, ncv, nmv);
    return curr_err;
}

/**
 * updateError helper function, checking horizons
 * @param {React.useState} sce MPC scenario hook
 * @param {React.useState} error Error hook
 */
function updateHorizons(sce, error) {
    const data = [parseInt(sce["T"]), parseInt(sce["P"]), parseInt(sce["M"]), parseInt(sce["W"])];
    // Check T: 
    if (Math.max(...data) === data[0] && data[0] > 0) {
        error["T"] = false;
    } else { error["T"] = true; }
    // Check P:
    if (data[0] > data[1] && data[1] >= data[2] && data[1] > data[3] && data[1] > 0) {
        error["P"] = false;
    } else { error["P"] = true; } 
    // Check W:
    (data[3] <= data[1] ? error["W"] = false : error["W"] = true) 
    // Check M:
    if (data[2] <= data[1] && data[2] < data[0] && data[2] > 0) {
        error["M"] = false;
    } else {
        error["M"] = true;
    }
}

/**
 * updateError helper function, checking tuning parameters
 * @param {React.useState} sce MPC scenario hook
 * @param {React.useState} error Error hook
 * @param {Number} ncv number of controlled variables
 * @param {Number} nmv number of manipulated variables
 */
function updateTunings(sce, error, ncv, nmv) {
    const ncv_arr = ["Q", "RoH", "RoL"];
    const nmv_arr = "R";
    
    tuning_arr.forEach((tuning) => {
        const data = convertArr(sce[tuning]);
        if (data.length === ncv) { // If size is correct
            if (data.some(v => v < 0) === false) { // Only positive elements
                error[tuning] = false;
            }
        } else {
            error[tuning] = true; 
        }
    })

    const data = convertArr(sce[nmv_arr]);
    if (data.length === ncv) { // If size is correct
        if (data.some(v => v < 0) === false) { // Only positive elements
            error[nmv_arr] = false;
        }
    } else {
        error[nmv_arr] = true; 
    }

}

/**
 * updateError helper function, checking constraints
 * @param {React.useState} sce MPC scenario hook
 * @param {React.useState} error Error hook
 * @param {Number} ncv number of controlled variables
 * @param {Number} nmv number of manipulated variables
 */
function updateConstraints(sce, error, ncv, nmv) {
    const constraint_arr = ["ldu", "lu", "ly", "udu", "uu", "uy"];
    const size_arr = [nmv, nmv, ncv];

    let data_arr = []; // Storing sce data
    constraint_arr.forEach((elem) => {
        data_arr.push(convertArr(sce[elem])); 
    })

    const c_size = constraint_arr.length / 2;
    for (let i = 0; i < c_size; i++) { // Iterating over constraint vec
        let lower = data_arr[i];
        let upper = data_arr[i + c_size];

        if ((lower.length !== size_arr[i]) || (upper.length !== size_arr[i])) { // If size incorrect
            error[constraint_arr[i]] = true;
            error[constraint_arr[i + c_size]] = true;
        } else {
            let num_correct = 0; 
            for (let j = 0; j < size_arr[i]; j++) { // Iterating over c element
                if (lower[j] < upper[j]) {
                    num_correct++; 
                }
            }
            if (num_correct === size_arr[i]) { // If every element as correct relation
                error[constraint_arr[i]] = false;
                error[constraint_arr[i + c_size]] = false;
            } else {
                error[constraint_arr[i]] = true;
                error[constraint_arr[i + c_size]] = true;
            }
        }
    }
}


