/**
 * @file openloop.cc
 * @author Geir Ola Tvinnereim
 * @brief module implementing open loop simulation
 * @version 0.1
 * @date 2023-06-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "openloop.h"
#include "IO/data_objects.h"
#include "IO/parse.h"
#include "IO/serialize.h"
#include "IO/json_specifiers.h"
#include "model/FSRModel.h"

#include <vector>
#include <map>
#include <Eigen/Dense>
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;

/**
 * @brief Produce general input for Open loop simulation. Reference becomes input for OpenLoop simulation.
 * 
 * @param ref_str reference string
 * @param T MPC horizon
 * @param step Determines how actuation shall step from 0 to reference input. 
 * @return Eigen::MatrixXd change in actuation.
 */
static MatrixXd setStepActuation(const string& ref_str, const string& step_str, int n_CV, int T) { 
    // Split string to std::vector
    std::vector<double> ref_vec = ParseRefString(ref_str, n_CV); 
    std::vector<double> step_vec = ParseRefString(step_str, n_CV); 

    MatrixXd du = MatrixXd::Zero(int(ref_vec.size()), T);
    for (int i = 0; i < du.rows(); i++) {
        double sum = 0;
        for (int j = 0; j < T; j++) {
            // Insert step or 0 if sum has not reached ref_vec:
            if (sum < ref_vec[i]) {
                du(i, j) = step_vec[i];
                sum += step_vec[i];
            } else {
                du(i, j) = 0;
            }
        }
    }
    return du;
}

void OpenLoopFSRM(const string& sys, const string& ref_str, const string& step_str, int T) {
    const string sim = "sim_open_loop_" + sys; // Simulation file name
    const string sim_path = "../data/simulations/" + sim + ".json";

    // System variables:
    CVData cvd; 
    MVData mvd;
    std::map<string, int> m_map;

    // Parse information:
    ParseOpenLoop(sys, m_map, cvd, mvd);
    FSRModel fsr(cvd.getSR(), m_map, mvd.Inits, cvd.getInits()); // Open loop constructor

    // Actuation: Reference in Open Loop Simulation becomes actuation.
    MatrixXd du = setStepActuation(ref_str, step_str, m_map[kN_CV], T); 
    MatrixXd u_mat = MatrixXd::Zero(fsr.getN_MV(), T), y_pred = MatrixXd::Zero(fsr.getN_CV(), T);
 
    for (int k = 0; k < T; k++) {
        u_mat.col(k) = fsr.getUK();
        y_pred.col(k) = fsr.getY(du.col(k));

        // Propagate FSR model:
        fsr.UpdateU(du.col(k));
    }

    // Serialize: 
    try { // Serialize open loop must be changed to fit the new parsing lib
        SerializeOpenLoop(sim_path, sys, cvd, mvd, y_pred, u_mat, fsr, T); 
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}