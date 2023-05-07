
import { convertArr } from './IO.js'

/**
 * Checks if array contains onlu numbers or number strings
 * @param {*} arr 
 * @returns 
 */
export function onlyNumbers(arr) { 
    return arr.every(element => {
        if (!isNaN(element) && element !== "0") {
            return true;
        } else {
            return false;
        }
    });
}

export function updateError(scenario, error, ncv, nmv) {
    var curr_err = error;

    updateHorizons(scenario, curr_err);
    updateTunings(scenario, curr_err, ncv);
    updateConstraints(scenario, curr_err, ncv, nmv);
    return curr_err;
}

function updateHorizons(scenario, error) {
    const data = [parseInt(scenario["T"]), parseInt(scenario["P"]), parseInt(scenario["M"]), parseInt(scenario["W"])];
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
    if (data[2] < data[1] && data[2] < data[0] && data[2] > 0) {
        error["M"] = false;
    } else {
        error["M"] = true;
    }
}

function updateTunings(scenario, error, ncv) {
    const tuning_arr = ["Q", "R", "RoH", "RoL"];
    
    tuning_arr.forEach((tuning) => {
        const data = convertArr(scenario[tuning]);
        if (data.length === ncv) { // If size is correct
            if (data.some(v => v < 0) === false) { // Only positive elements
                error[tuning] = false;
            }
        } else {
            error[tuning] = true; 
        }
    })
}

function updateConstraints(scenario, error, ncv, nmv) {
    const constraint_arr = ["ldu", "lu", "ly", "udu", "uu", "uy"];
    const size_arr = [nmv, nmv, ncv];
    let data_arr = [];

    constraint_arr.forEach((elem) => {
        data_arr.push(convertArr(scenario[elem])); 
    })
    const c_size = constraint_arr.length / 2;
    for (let i = 0; i < c_size; i++) { // Iterating over constraint vec
        let lower = data_arr[i];
        let upper = data_arr[i + c_size];

        if ((lower.length !== size_arr[i]) || (upper.length !== size_arr[i])) { // Size incorrect
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


