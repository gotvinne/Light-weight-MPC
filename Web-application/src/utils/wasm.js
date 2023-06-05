import mpc_simulator from "../mpc_simulator.mjs";

/**
 * Calling Webassembly compiled C++ simulation
 * @param {String} sce_file Scenario file
 * @param {String} sys_file System file
 * @param {String} sce Scenario name
 * @param {Number} T MPC horizon
 * @param {React.useState} setSimHook Simulation hook
 */
export function simulate(sce_file, sys_file, sce, ref_str, T, setSimHook, errorHook) {
    let wasm: any;
    mpc_simulator()
    .then((module) => {
        wasm = module;
        setSimHook(wasm.simulate(sce_file, sys_file, sce, ref_str, T));
    })
    .catch((error) => {
        console.error(error);
        errorHook(simStatus => { return {...simStatus, error: String(error)}});
        setSimHook("SIM ERROR");
    });
}