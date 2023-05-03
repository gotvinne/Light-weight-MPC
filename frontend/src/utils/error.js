
import { convertArr } from './IO.js'

export function updateError(scenario, error, ncv, nmv) {
    // scenario:
    // 0 = System, 1 = Scenario
    // 2 = T, 3 = P, 4 = M, 5 = W
    // 6 = Q, 7 = R, 8 = RoH, 9 = RoL
    // 10 = ldu, 11 = lu, 12 = ly
    // 13 = udu, 14 = uu, 15 = uy
    var curr_err = error;

    updateHorizons(scenario, curr_err);
    updateTunings(scenario, curr_err, ncv);
    updateConstraints(scenario, curr_err, ncv, nmv);
    console.log(curr_err);
    return curr_err;
}

function updateHorizons(scenario, error) {
    const data = [parseInt(scenario["T"]), parseInt(scenario["P"]), parseInt(scenario["M"]), parseInt(scenario["W"])];
    console.log(data);
    // Check T: 
    if (Math.max(...data) === data[0] && data[0] > 0) {
        error["T"] = false;
    } else {
        error["T"] = true;
    }
    // Check P:
    if (data[0] > data[1] && data[1] >= data[2] && data[1] > data[3] && data[1] > 0) {
        error["P"] = false;
    } else {
        error["P"] = true;    
    } 
    // Check W
    if (data[3] <= data[1]) {
        error["W"] = false;
    } else {
        error["W"] = true;    
    }
    // Check M
    if (data[2] < data[1] && data[2] < data[0] && data[2] > 0) {
        error["M"] = false;
    } else {
        error["M"] = true;
    }
}

function updateTunings(scenario, error, ncv) {
    const tuning_arr = ["Q", "R", "RoH", "RoL"];
    
    tuning_arr.forEach((tuning) => {
        var data = convertArr(scenario[tuning]);
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
    const size_arr = [nmv, nmv, ncv, nmv, nmv, ncv];

    constraint_arr.forEach((elem, index) => {
        var data = convertArr(scenario[elem]); 
        // Might add check to see if upper bounds are greater then lower
        if (data.length=== size_arr[index]) { // Checks size
            error[elem] = false;
        } else {
            error[elem] = true;
        }
    })
}


