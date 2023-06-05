/** JSON formatting module, parsing systems, serializing scenario and simulation */

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
  } else { // If many elements
    return arr.split(",").map(Number);
  }
}

/** SYSTEM IO*/

/**
 * 
 * @param {Array} l_arr lower constraints 
 * @param {Array} u_arr upper constraints
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
 * @returns array of avaliable systems
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
 * @param {string} identifier CV/MV identifier
 * @return array, first index is CV/MV data second is corresponding units
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
 * Read JSON file from local system folder
 * @param {string} fileName 
 * @return JSON formatted Object
 */
export function readSystem(fileName) {
  const resource = require(`./../systems/${fileName}.json`); // Load file
  return JSON.stringify(resource);
}

/** SCENARIO IO*/

/**
 * Serialize Scenario JSON file based on scenario
 * @param {React.useState} sce 
 * @return Serialized scenario JSON object
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
      "P": parseInt(sce["P"]), 
      "M": parseInt(sce["M"]),
      "W": parseInt(sce["W"]), 
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
  let number_arr = ref.map(Number);
  
  const json_ref = {
    "ref": number_arr
  };
  return JSON.stringify(json_ref);
}

/** SIMULATION IO */
/**
 * Read model parameters
 * @param {String} sim JSON simulation
 * @returns Object of model parameters
 */
export function readSimModelParams(sim) {
    const simParam = {
    scenario: sim["scenario"],
    T: sim["T"],
    nCV: sim["n_CV"],
    nMV: sim["n_MV"],
    P: sim["P"],
    M: sim["M"]
  }
  return simParam; 
}

/**
 * Read CVs from simulation file
 * @param {String} sim JSON simulation
 * @returns Array of CV data
 */
export function readSimCV(sim) {
  const CVs = []; 
  const cv_data = sim["CV"]; 

  cv_data.forEach((data) => {
      const elem = {
        output: data["output"],
        unit: data["unit"],
        c: data["c"],
        y_pred: data["y_pred"],
        ref: data["ref"]
      }
      CVs.push(elem); // Store CV objects
    }
  ); 
  return CVs;
}

/**
 * Read MVs from simulation file
 * @param {String} sim JSON simulation
 * @returns Array of MV data
 */
export function readSimMV(sim) {
  const MVs = []; 
  const mv_data = sim["MV"];

  mv_data.forEach((data) => {
      const elem = {
        input: data["input"],
        unit: data["unit"],
        c: data["c"],
        u: data["u"]
      }
      MVs.push(elem); // Store MV objects
    }
  );
  return MVs;
}
