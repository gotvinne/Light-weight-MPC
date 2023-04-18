
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