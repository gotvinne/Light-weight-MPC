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
 * @brief Argument checking for assuring valid open loop sim
 * 
 * @param ref_str Reference vector
 * @param step_str step vector
 * @param n_CV Number of controlled variables
 */
static void CheckArgs(const std::vector<double>& ref_vec, const std::vector<double>& step_vec, int n_CV) {
    if (ref_vec.size() != n_CV) {
        throw std::invalid_argument("Number of reference elements does not coincide with n_CV");
    } else if (step_vec.size() != n_CV) {
        throw std::invalid_argument("Number of step elements does not coincide with n_CV");
    }
    
    for (int i = 0; i < n_CV; i++) {
        if (step_vec[i] > ref_vec[i]) {
            throw std::invalid_argument("Step element "+std::to_string(i)+", is larger then reference element");
        } else if (int(ref_vec[i]) % int(step_vec[i]) != 0) {
            throw std::invalid_argument("Step element "+std::to_string(i)+", does not have zero reminder in division with the reference element");
        }
    }
}

/**
 * @brief Produce general input for Open loop simulation. Reference becomes input for OpenLoop simulation.
 * 
 * @param ref_str reference string
 * @param step_str step string
 * @param n_CV number of controlled variables
 * @param T MPC horizon 
 * @return Eigen::MatrixXd change in actuation.
 */
static MatrixXd setStepActuation(const string& ref_str, const string& step_str, int n_CV, int T) { 
    // Split string to std::vector
    std::vector<double> ref_vec = ParseRefString(ref_str); 
    std::vector<double> step_vec = ParseRefString(step_str); 
    CheckArgs(ref_vec, step_vec, n_CV);

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
    try {
        ParseOpenLoop(sys, m_map, cvd, mvd);
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        exit(1);
    }
   
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
        exit(1);
    }
}