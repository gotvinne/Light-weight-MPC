import mpc_simulator from "../mpc_simulator.mjs";

/**
 * Calling Webassembly compiled C++ simulation
 * @param {String} sce_file Scenario file
 * @param {String} sys_file System file
 * @param {String} sce Scenario name
 * @param {Number} T MPC horizon
 * @param {React.useState} setSimHook Simulation hook
 * @param {React.useState} setSimError Simulation status hook
 */
export function simulate(sce_file, sys_file, sce, ref_str, T, setSimHook, setSimError) {
    let wasm;
    mpc_simulator()
    .then((module) => {
        wasm = module;
        setSimHook(wasm.simulate(sce_file, sys_file, sce, ref_str, T));
    })
    .catch((error) => {
        console.error('Error:', error.message);
        setSimError(error.message);
        setSimHook("SIM ERROR");
    });
}
