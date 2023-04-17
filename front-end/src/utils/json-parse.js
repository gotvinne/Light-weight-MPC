
const output = "output";

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
  console.log(resource["module"]);
}

/** Read system file */
export function readSystem(fileName, setHook) {
  const resource = require(`./../systems/${fileName}/.json`); // Load file
  setHook(JSON.parse(JSON.stringify(resource)));
}