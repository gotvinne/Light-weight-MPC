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
export function updateError(sce, error, ncv, nmv, n) {
    var curr_err = error;

    updateHorizons(sce, curr_err, n);
    updateTunings(sce, curr_err, ncv, nmv);
    updateConstraints(sce, curr_err, ncv, nmv);
    return curr_err;
}

/**
 * updateError helper function, checking horizons
 * @param {React.useState} sce MPC scenario hook
 * @param {React.useState} error Error hook
 * @param {Number} N Model horizon
 */
function updateHorizons(sce, error, N) {
    const T = parseInt(sce["T"]), P = parseInt(sce["P"]), M = parseInt(sce["M"]), W= parseInt(sce["W"]);
    // Check T: 
    (T > 0) ? error["T"] = false : error["T"] = true;
    
    // Check P:
    (P <= N && P >= M && P > 0) ? error["P"] = false : error["P"] = true; 
    
    // Check W:
    (W <= M && W >= 0) ? error["W"] = false : error["W"] = true;

    // Check M:
    (M <= P && M > 0) ? error["M"] = false : error["M"] = true;
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
    
    const disable_slack = DisableSlack(sce, error);
    if (disable_slack) {
        // Check only Q
        const data = convertArr(sce["Q"]);
        if (data.length === ncv) { // If size is correct
            if (data.some(v => v < 0) === false) { // Only positive elements
                error["Q"] = false;
            }
        } else {
            error["Q"] = true; 
        }
    } else {
        ncv_arr.forEach((tuning) => {
            const data = convertArr(sce[tuning]);
            if (data.length === ncv) { // If size is correct
                if (data.some(v => v < 0) === false) { // Only positive elements
                    error[tuning] = false;
                }
            } else {
                error[tuning] = true; 
            }
        })
    }
    
    // Check R:
    const data = convertArr(sce["R"]);
    if (data.length === nmv) { // If size is correct
        if (data.some(v => v < 0) === false) { // Only positive elements
            error["R"] = false;
        }
    } else {
        error["R"] = true; 
    }
}

/**
 * Determine slack disabled
 * @param {React.useState} sce MPC scenario hook
 * @param {React.useState} error Error hook
 * @returns bool is slack is disabled
 */
function DisableSlack(sce, error) {
    const roh = convertArr(sce["RoH"]), rol = convertArr(sce["RoL"]);
    if (roh.length === 0 && rol.length === 0) {
        error["RoH"] = false;
        error["RoL"] = false;
        return true;
    } else {
        return false;
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


