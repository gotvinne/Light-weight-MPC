import backend from "../webassembly.mjs";

/**
 * Calling Webassembly compiled C++ simulation
 * @param {string} sce_file 
 * @param {string} sys_file 
 * @param {string} sce 
 * @param {number} T 
 * @param {React.useState} setHook 
 */
export function simulate(sce_file, sys_file, sce, ref_str, T, setHook, errorHook) {
    let wasm: any;
    backend()
    .then((module) => {
        wasm = module;
        setHook(wasm.simulate(sce_file, sys_file, sce, ref_str, T));
    })
    .catch((error) => {
        console.error(error);
        errorHook(simStatus => { return {...simStatus, error: String(error)}});
    });
}