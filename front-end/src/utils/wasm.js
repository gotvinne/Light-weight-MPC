
import backend from "../webassembly.mjs";

/**
 * 
 * @param {*} sce_file 
 * @param {*} sys_file 
 * @param {*} sce 
 * @param {*} T 
 * @param {*} setHook 
 */
export function simulate(sce_file, sys_file, sce, T, setHook) {

    let wasm: any;
    backend()
    .then((module) => {
        wasm = module;
        setHook(wasm.simulate(sce_file, sys_file, sce, T));
    })
    console.log("Simulation done");
}