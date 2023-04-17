
/** Read all json files from system folder */
export function importSystems(systems, setSystems) {
    const context = require.context('./../systems', false, /.json$/); // Read files in folder.
    const set = new Set();
    for (let i = 0; i < context.keys().length; i++) {
        const key = context.keys()[i];
        const fileName = key.replace('./', '');
        //const resource = require(`./../systems/${fileName}`);
        //const namespace = fileName.replace('.json', '');
        set.add(fileName);
    }  
    setSystems(set);
}

//export function importSystems(systems) {
//    const context = require.context('./../systems', false, /.json$/); // Read files in folder.
//    for (let i = 0; i < context.keys().length; i++) {
       // const key = context.keys()[i];
      //  const fileName = key.replace('./', '');
        //const resource = require(`./../systems/${fileName}`);
        //const namespace = fileName.replace('.json', '');
    //    systems.add(fileName);
  //  }  
//}