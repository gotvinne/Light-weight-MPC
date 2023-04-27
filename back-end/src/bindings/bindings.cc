/**
 * @file bindings.cc
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

#include "IO/data_objects.h"
#include "IO/json_specifiers.h"
#include "IO/parse.h"
#include "IO/serialize.h"
#include "MPC/solvers.h"
#include "model/FSRModel.h"
#include "LightWeightMPC.h"

#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <Eigen/Dense>

using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

const double GAS_RATE_REF = 3800;
const double OIL_RATE_REF = 70;

string simulate(string sce_file, string sys_file, string sce, int T) {
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

    std::vector<double> ref_vec{GAS_RATE_REF, OIL_RATE_REF};
    // Reference: 
    if (int(ref_vec.size()) != m_map[kN_CV]) {
         return "Number of references do not coincide with constrained variables";
    }
    VectorXd* y_ref = AllocateConstReference(ref_vec, T, conf.P);

    // Solver: 
    try {
        SRSolver(T, u_mat, y_pred, fsr, conf, z_min, z_max, y_ref);
        delete[] y_ref;
    }
    catch(std::exception& e) {
        return string(e.what());
    }
    
    //return "Works!";
    return SerializeSimulation(sce, cvd, mvd, 
              y_pred, u_mat, z_min, z_max, fsr, T);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("simulate", &simulate);
}
#endif // __EMSCRIPTEN__
