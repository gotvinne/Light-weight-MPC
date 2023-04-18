
function convertArr(str_arr) {

  // Removing brackets: 
  let arr = str_arr.replace('[', '');
  arr = arr.replace(']', '');

  return arr.split(",").map(Number);
}

function processConstraints(l_arr, u_arr, identifier) {
  if (l_arr.length !== u_arr.length) {
    throw new Error("Invalid number of constraints!");
  }

  let lst = [];
  for (let i = 0; i < l_arr.length; i++) {
    let obj = new Object();
    const key = "".concat(identifier, "[", i, "]");
    obj[key] = [l_arr[i], u_arr[i]];
    lst.push(obj);
  }
  return lst;
}

/** Read all json filenames from system folder */
export function importSystems(setHook) {
    const context = require.context('./../systems', false, /.json$/); // Read files in folder.
    const set = new Set();
    for (let i = 0; i < context.keys().length; i++) {
        const key = context.keys()[i];
        const fileName = key.replace('./', '');
        
        const namespace = fileName.replace('.json', '');
        set.add(namespace);
    }  
    setHook(Array.from(set));
}

/**  Read system file model params */
export function readModelParams(fileName, setHook, identifier) {
  const resource = require(`./../systems/${fileName}.json`); // Load file
  const data = resource[identifier];
 
  const set = new Set();

  if (identifier === "CV") {
    data.forEach((cv) => {
      set.add(cv["output"]);
    });
  } else if (identifier === "MV") {
    data.forEach((mv) => {
      set.add(mv["input"]);
    });
  } else {
    throw new Error("Invalid identifier!");
  }
  
  setHook(Array.from(set));
}

/** Read system file */
export function readSystem(fileName, setHook) {
  const resource = require(`./../systems/${fileName}.json`); // Load file
  setHook(JSON.stringify(resource));
}

/** Constructing scenario json file */
export function serializeScenario(tuning, setHook) {
 
  const q_arr = convertArr(tuning["Q"]); 
  const r_arr = convertArr(tuning["R"]);

  const roh_arr = convertArr(tuning["RoH"]);
  const rol_arr = convertArr(tuning["RoL"]);

  const ldu_arr = convertArr(tuning["ldu"]);
  const lu_arr = convertArr(tuning["lu"]);
  const ly_arr = convertArr(tuning["ly"]);

  const udu_arr = convertArr(tuning["udu"]);
  const uu_arr = convertArr(tuning["uu"]);
  const uy_arr = convertArr(tuning["uy"]);

  const scenario = {
    "system": tuning["System"],

    "MPC": {
      "P": tuning["P"], 
      "M": tuning["M"],
      "W": tuning["W"], 
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

  setHook(JSON.stringify(scenario));
}

