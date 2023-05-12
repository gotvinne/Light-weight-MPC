/**
 * @file simulations.cc
 * @author Geir Ola Tvinnereim
 * @brief Script for simulation functionalities
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Released under the terms of the BSD 3-Clause License
 * 
 */
#include "simulations.h"
#include "MPC/solvers.h"

#include "IO/json_specifiers.h"
#include "IO/data_objects.h"
#include "IO/parse.h"
#include "IO/serialize.h"

#include <iostream>

/**
 * @brief Produce general input for Open loop simulation. Reference becomes input for OpenLoop simulation.
 * 
 * @param ref_str reference string
 * @param T MPC horizon
 * @param step Determines how actuation shall step from 0 to reference input. 
 * @return Eigen::MatrixXd change in actuation.
 */
static MatrixXd setStepActuation(const string& ref_str, const std::vector<double>& step, int n_CV, int T) { 
    // Split string to std::vector
    std::vector<double> ref_vec = ParseRefString(ref_str, n_CV); 
    MatrixXd du = MatrixXd::Zero(int(ref_vec.size()), T);
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
    return du;
}

/**
 * @brief set Reference variable based on string
 * 
 * @param ref_str reference string
 * @param T MPC horizon
 * @param P Prediction horizon
 * @param n_CV number of controlled variables
 * @return MatrixXd reference variable
 */
static MatrixXd setRef(const string& ref_str, int T, int P, int n_CV) {
    // ref = R^(n_CV x T + P)
    // Split string to std::vector
    std::vector<double> ref_vec = ParseRefString(ref_str, n_CV); 
    
    int size = int(ref_vec.size());
    MatrixXd ref = MatrixXd::Zero(size, T + P);
    if (size != n_CV) {
        throw std::invalid_argument("Number of references do not coincide with number of n_CV, expected: " + std::to_string(n_CV));
    }

    for (int i = 0; i < size; i++) {
        ref.row(i) = VectorXd::Constant(T + P, ref_vec.at(i)); // Takes predictions into account!
    }
    return ref;
}

void OpenLoopFSRM(const string& sys, const string ref_str, int T) {
    const string sim = "sim_open_loop_" + sys; // Simulation file name
    const string sim_path = "../data/simulations/" + sim + ".json";

    // System variables:
    CVData cvd; 
    MVData mvd;
    std::map<string, int> m_map;

    // Scenario variables:
    VectorXd z_min, z_max; /** Constraint vectors */
    MPCConfig conf; /** MPC configuration */

    // Parse information:
    ParseOpenLoop(sys, m_map, cvd, mvd);

    // Select dynamical model: 
    FSRModel fsr(cvd.getSR(), m_map, mvd.Inits, cvd.getInits());

    // Actuation: Reference in Open Loop Simulation becomes actuation.
    std::vector<double> step {5, 25};
    MatrixXd du = setStepActuation(ref_str, step, m_map[kN_CV], T); 
    MatrixXd u_mat = MatrixXd::Zero(fsr.getN_MV(), T), y_pred = MatrixXd::Zero(fsr.getN_CV(), T);
 
    for (int k = 0; k < T; k++) {
        // Store optimal du and y_pref: Before update!
        u_mat.col(k) = fsr.getUK();
        y_pred.col(k) = fsr.getY(du.col(k));

        // Propagate FSR model:
        fsr.UpdateU(du.col(k));
    }

    // Serialize:
    try {
        SerializeOpenLoop(sim_path, sys, cvd, mvd, y_pred, u_mat, fsr, T); 
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void MPCSimFSRM(const string& sys, const string& ref_vec, bool new_sim, int T) {
    // Mapping to Data folder
    const string sim = "sim_" + sys;
    const string sce_path = "../data/scenarios/sce_" + sys + ".json";
    const string sim_path = "../data/simulations/" + sim + ".json";

    // System variables:
    CVData cvd; 
    MVData mvd;
    std::map<string, int> m_map;

    // Scenario variables:
    VectorXd z_min, z_max; /** Constraint vectors */
    MPCConfig conf; 
    MatrixXd du_tilde; 

    // Parse information:
    try {
        if (new_sim) {
            ParseNew(sce_path, m_map, cvd, mvd, conf, z_min, z_max);
            du_tilde = MatrixXd::Zero(m_map[kN_MV], m_map[kN]-1);
        } else {
            Parse(sce_path, sim_path, m_map, cvd, mvd, conf, z_min, z_max, du_tilde); // Check du thilde here
        }
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    
    // FSRM:
    FSRModel fsr(cvd.getSR(), m_map, conf.P, conf.M, conf.W, mvd.Inits, cvd.getInits());
    fsr.setDuTildeMat(du_tilde); 

    // MPC variables:
    MatrixXd u_mat, y_pred, ref = setRef(ref_vec, T, conf.P, m_map[kN_CV]); 
    /** Optimized actuation, (n_MV, T) */ /** Predicted output (n_CV, T)*/ /** Reference */

    try { // Solve
        SRSolver(T, u_mat, y_pred, fsr, conf, z_min, z_max, ref);

        if (new_sim) { // Serialize
            SerializeSimulationNew(sim_path, sys, cvd, mvd, 
               y_pred, u_mat, z_min, z_max, ref, fsr, T);
        } else {
            SerializeSimulation(sim_path, y_pred, u_mat, ref, T);
        }
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}