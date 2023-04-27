/**
 * @file LightWeightMPC.cc
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Released under the terms of the BSD 3-Clause License
 * 
 */
#include "LightWeightMPC.h"
#include "MPC/solvers.h"
#include "IO/json_specifiers.h"
#include "IO/data_objects.h"
#include "IO/parse.h"
#include "IO/serialize.h"

#include <iostream>

/**
 * @brief 
 * 
 * @param du 
 * @param ref_vec 
 * @param T 
 * @param step 
 */
static void AllocateStepReference(MatrixXd& du, const std::vector<double>& ref_vec, const std::vector<double>& step, int T) { 
    du.resize(int(ref_vec.size()), T);
    for (int i = 0; i < du.rows(); i++) {
        double sum = 0;
        for (int j = 0; j < T; j++) {
            
            if (sum < ref_vec[i]) {
                du(i, j) = step[i];
            } else {
                du(i, j) = 0;
            }
            sum += step[i];
        }
    }
}

VectorXd* AllocateConstReference(const std::vector<double>& ref_vec, int T, int P) { // Cannot reinitialize pointer via pass-by-pointer
    VectorXd* y_ref = new VectorXd[int(ref_vec.size())];

    for (int i = 0; i < int(ref_vec.size()); i++) {
        y_ref[i] = VectorXd::Constant(T + P, ref_vec.at(i)); // Takes predictions into account!
    }
    return y_ref;
}

void OpenLoopSim(const string& system, const std::vector<double>& ref_vec, int T) {
    const string sim = "sim_open_loop_" + system;
    const string sim_path = "../data/simulations/" + sim + ".json";

    // System variables:
    CVData cvd; 
    MVData mvd;
    std::map<string, int> m_map;

    // Scenario variables:
    VectorXd z_min; /** Lower constraint vector */
    VectorXd z_max; /** Upper constraint vector */
    MPCConfig conf; /** MPC configuration */

    // Parse information:
    ParseOpenLoop(system, m_map, cvd, mvd);

    // Select dynamical model: 
    FSRModel fsr(cvd.getSR(), m_map, mvd.Inits, cvd.getInits());

    // Actuation: Interface to FSRModel is change in actuation.
    MatrixXd du;
    std::vector<double> step {5, 25};
    AllocateStepReference(du, ref_vec, step, T); 
    
    MatrixXd u_mat = MatrixXd::Zero(fsr.getN_MV(), T);
    MatrixXd y_pred = MatrixXd::Zero(fsr.getN_CV(), T);

    for (int k = 0; k < T; k++) {
        // Store optimal du and y_pref: Before update!
        u_mat.col(k) = fsr.getUK();
        y_pred.col(k) = fsr.getY(du.col(k));

        // Propagate FSR model:
        fsr.UpdateU(du.col(k));
    }

    // Serialize:
    SerializeOpenLoop(sim_path, system, cvd, mvd, y_pred, u_mat, fsr, T); 
}

void LightWeightMPC(const string& sce, const std::vector<double>& ref_vec, bool new_sim, int T) {
    const string sim = "sim_" + sce;
    const string sce_path = "../data/scenarios/sce_" + sce + ".json";
    const string sim_path = "../data/simulations/" + sim + ".json";

    // System variables:
    CVData cvd; 
    MVData mvd;
    std::map<string, int> m_map;

    // Scenario variables:
    VectorXd z_min; /** Lower constraint vector */
    VectorXd z_max; /** Upper constraint vector */
    MPCConfig conf; /** MPC configuration */
    MatrixXd du_tilde; 

    // Parse information:
    try {
        if (new_sim) {
            ParseNew(sce_path, m_map, cvd, mvd, conf, z_min, z_max);
            du_tilde = MatrixXd::Zero(m_map[kN_MV], m_map[kN]-conf.W-1);
        } else {
            Parse(sce_path, sim_path, m_map, cvd, mvd, conf, z_min, z_max, du_tilde);
        }
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    
    // Select dynamical model: 
    FSRModel fsr(cvd.getSR(), m_map, conf.P, conf.M, conf.W, mvd.Inits, cvd.getInits());
    fsr.setDuTildeMat(du_tilde);

    // MPC variables:
    MatrixXd u_mat; /** Optimized actuation, (n_MV, T) */
    MatrixXd y_pred; /** Predicted output (n_CV, T)*/

    // Reference: 
    if (int(ref_vec.size()) != m_map[kN_CV]) {
        throw std::invalid_argument("Number of references do not coincide with constrained variables");
    }
    VectorXd* y_ref = AllocateConstReference(ref_vec, T, conf.P);

    // Solver: 
    try {
        SRSolver(T, u_mat, y_pred, fsr, conf, z_min, z_max, y_ref);
        delete[] y_ref;
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << SerializeSimulation(sce, cvd, mvd, 
               y_pred, u_mat, z_min, z_max, fsr, T) << std::endl;

    // Serializing: 
    //if (new_sim) {
        //SerializeSimulationNew(sim_path, sce, cvd, mvd, 
        //       y_pred, u_mat, z_min, z_max, fsr, T);
    //} else {
     //   SerializeSimulation(sim_path, y_pred, u_mat, T);
    //}
}