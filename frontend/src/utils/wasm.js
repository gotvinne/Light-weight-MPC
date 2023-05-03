
import backend from "../webassembly.mjs";

/**
 * 
 * @param {string} sce_file 
 * @param {string} sys_file 
 * @param {string} sce 
 * @param {number} T 
 * @param {React.useState} setHook 
 */
export function simulate(sce_file, sys_file, sce, ref_str, T, setHook) {
    let wasm: any;
    backend()
    .then((module) => {
        wasm = module;
        setHook(wasm.simulate(sce_file, sys_file, sce, ref_str, T));
    })
    .catch((error) => {
        console.error(error);
    });
}