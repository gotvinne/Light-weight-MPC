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
    VectorXd z_min; /** Lower constraint vector */
    VectorXd z_max; /** Upper constraint vector */
    MPCConfig conf; /** MPC configuration */

    // Parse information:
    try {
        Parse(sce_file, sys_file, m_map, cvd, mvd, conf, z_min, z_max);
    }
    catch(std::exception& e) {
        return string(e.what());
    }
    MatrixXd du_tilde = MatrixXd::Zero(m_map[kN_MV], m_map[kN]-conf.W-1);

    // Select dynamical model: 
    FSRModel fsr(cvd.getSR(), m_map, conf.P, conf.M, conf.W, mvd.Inits, cvd.getInits());
    fsr.setDuTildeMat(du_tilde);

    // MPC variables:
    MatrixXd u_mat; /** Optimized actuation, (n_MV, T) */
    MatrixXd y_pred; /** Predicted output (n_CV, T)*/

    // Reference: 
    VectorXd* y_ref = ParseReferenceStrByAllocation(ref_str, T, conf.P);

    // Solver: 
    try {
        SRSolver(T, u_mat, y_pred, fsr, conf, z_min, z_max, y_ref);
        delete[] y_ref;
    }
    catch(std::exception& e) {
        return string(e.what());
    }
    
    return SerializeSimulation(sce, cvd, mvd, 
              y_pred, u_mat, z_min, z_max, fsr, T);
}

// webassembly.mjs: Functions added into EMCRIPTEN_BINDINGS are compiled to JS using Webassembly.
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("simulate", &simulate);
}
#endif // __EMSCRIPTEN__
