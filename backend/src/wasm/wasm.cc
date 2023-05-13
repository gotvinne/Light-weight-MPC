/**
 * @file wasm.cc
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright  Released under the terms of the BSD 3-Clause License
 * 
 */
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#endif // __EMSCRIPTEN__

#include "wasm/wasm.h"

string simulate(string sce_file, string sys_file, string sce, string ref_str, int T) {
    // System variables:
    CVData cvd; 
    MVData mvd;
    std::map<string, int> m_map;

    // Scenario variables:
    VectorXd z_min, z_max; /** Constraint vectors */
    MPCConfig conf; /** MPC configuration */

    // Parse information:
    try {
        Parse(sce_file, sys_file, m_map, cvd, mvd, conf, z_min, z_max);
    }
    catch(std::exception& e) {
        return string(e.what());
    }

    // FSRM:
    MPCConfig sim_conf = conf;
    FSRModel* fsr_cost;
    bool reduced_cost = (conf.W != 0);
    if (reduced_cost) {
        fsr_cost = new FSRModel(cvd.getSR(), m_map, conf, mvd.Inits, cvd.getInits());
        sim_conf.W = 0;
    } 
    FSRModel fsr_sim(cvd.getSR(), m_map, sim_conf, mvd.Inits, cvd.getInits());

    // MPC variables:
    MatrixXd u_mat, y_pred, ref = ParseReferenceStr(ref_str, T, conf.P);

    try { // Solve:
        if (reduced_cost) {
            SRSolver(T, u_mat, y_pred, fsr_sim, *fsr_cost, conf, z_min, z_max, ref);
            delete fsr_cost;
        } else {
            SRSolver(T, u_mat, y_pred, fsr_sim, conf, z_min, z_max, ref);
        }
    }
    catch(std::exception& e) {
        return string(e.what());
    }
    
    return SerializeSimulation(sce, cvd, mvd, 
              y_pred, u_mat, z_min, z_max, ref, fsr_sim, T);
}

// webassembly.mjs: Functions added into EMCRIPTEN_BINDINGS are compiled to JS using Webassembly.
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("simulate", &simulate);
}
#endif // __EMSCRIPTEN__
