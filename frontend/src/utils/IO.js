/** SYSTEM IO */

/**
 * Convert string array to number array
 * @param {Array} str_arr 
 * @returns Array
 */
export function convertArr(str_arr) {

  // Removing brackets: 
  let arr = str_arr.replace('[', '');
  arr = arr.replace(']', '');
  
  if (arr === '') { // If empty list
    return [];
  } else if (!isNaN(arr)) { // If only one number
    return [parseFloat(arr)];
  } else { // For many variables
    return arr.split(",").map(Number);
  }
}

/**
 * 
 * @param {Array} l_arr 
 * @param {Array} u_arr 
 * @param {string} identifier 
 * @returns Array
 */
function processConstraints(l_arr, u_arr, identifier) {
  if (l_arr.length !== u_arr.length) {
    throw new Error("Invalid number of constraints!");
  }

  let lst = [];
  for (let i = 0; i < l_arr.length; i++) {
    let obj = {};
    const key = "".concat(identifier, "[", i, "]");
    obj[key] = [l_arr[i], u_arr[i]];
    lst.push(obj);
  }
  return lst;
}

/**
 * Import avaliable system filenames
 */
export function importSystems() {
    const context = require.context('./../systems', false, /.json$/); // Read files in folder.
    const arr = [];
    for (let i = 0; i < context.keys().length; i++) {
        const key = context.keys()[i];
        const fileName = key.replace('./', '');
        
        const namespace = fileName.replace('.json', '');
        arr.push(namespace);
    }  
    return arr;
}

/**
 * Read model parameters from system JSON file
 * @param {string} fileName 
 * @param {string} identifier 
 */
export function readModelParams(fileName, identifier) {
  const resource = require(`./../systems/${fileName}.json`); // Load file
  const data = resource[identifier];
 
  const arr = [];
  const units = [];
  if (identifier === "CV") {
    data.forEach((cv) => {
      arr.push(cv["output"]);
      units.push("["+ cv["unit"] +"]");
    });
  } else if (identifier === "MV") {
    data.forEach((mv) => {
      arr.push(mv["input"]);
      if (mv["unit"] === "%") {
        units.push("[\\"+mv["unit"]+"]");
      } else {
        units.push("["+ mv["unit"] +"]");
      }
    });
  } else {
    throw new Error("Invalid identifier!");
  }
  
  return [arr, units];
}

/**
 * Read JSON file from systems database
 * @param {string} fileName 
 */
export function readSystem(fileName) {
  const resource = require(`./../systems/${fileName}.json`); // Load file
  return JSON.stringify(resource);
}

/** SCENARIO IO*/

/**
 * Serialize Scenario JSON file based on scenario
 * @param {React.useState} sce 
 */
export function serializeScenario(sce) {
 
  const q_arr = convertArr(sce["Q"]); 
  const r_arr = convertArr(sce["R"]);
  const roh_arr = convertArr(sce["RoH"]);
  const rol_arr = convertArr(sce["RoL"]);

  const ldu_arr = convertArr(sce["ldu"]);
  const lu_arr = convertArr(sce["lu"]);
  const ly_arr = convertArr(sce["ly"]);
  const udu_arr = convertArr(sce["udu"]);
  const uu_arr = convertArr(sce["uu"]);
  const uy_arr = convertArr(sce["uy"]);

  const scenario = {
    "system": sce["System"],

    "MPC": {
      "P": sce["P"], 
      "M": sce["M"],
      "W": sce["W"], 
      "Q": q_arr,
      "R": r_arr,
      "RoH": roh_arr,
      "RoL": rol_arr,
      "bias update": false
    },

    "c": [].concat(processConstraints(ldu_arr, udu_arr, "du"),
      processConstraints(lu_arr, uu_arr, "u"),
      processConstraints(ly_arr, uy_arr, "y"))
  }

  const json = JSON.stringify(scenario);
  return json;
}

export function serializeRef(ref) {
  const json_ref = {
    "ref": ref
  };
  return JSON.stringify(json_ref);
}

/** SIMULATION IO */
export function readSimParams(sim) {
    const simParam = {
    scenario: sim["scenario"],
    T: sim["T"],
    nCV: sim["n_CV"],
    nMV: sim["n_MV"]
  }
  return simParam; 
}

export function readSimCV(sim) {
  const CVs = []; // Declare array of CV
  const cv_data = sim["CV"]; // Array

  cv_data.forEach((data) => {
      const elem = {
        output: data["output"],
        unit: data["unit"],
        c: data["c"],
        y_pred: data["y_pred"]
      }
      CVs.push(elem);
    }
  );
  return CVs;
}

export function readSimMV(sim) {
  const MVs = []; // Declare array of CV
  const mv_data = sim["MV"]; // Array

  mv_data.forEach((data) => {
      const elem = {
        input: data["input"],
        unit: data["unit"],
        c: data["c"],
        u: data["u"]
      }
      MVs.push(elem);
    }
  );
  return MVs;
}
