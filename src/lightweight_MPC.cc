/**
 * @file light_weight_MPC.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "IO/serialize.h"
#include "IO/parse.h"
#include "IO/data_objects.h"
#include "OSQP-Eigen/sr_solver.h"
#include "model/step_response_model.h"
#include "model/FSRModel.h"

#include <map>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include <Eigen/Dense>
using json = nlohmann::json; 
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

void LightWeightMPC(int T) {
    string sys_path = "../data/systems/sr_siso_test.json";
    string sce_path = "../data/scenarios/siso_test.json";

    // System variables
    std::map<string, int> m_map;
    CVData sd;
    MVData id;

    // Scenario variables:
    string sys; 
    MPCConfig conf; //Default initializer
    VectorXd z_max;
    VectorXd z_min; 

    // Parse
    Parse(sys_path, sce_path, m_map, sd, id, sys, 
            conf, z_min, z_max, T);

    // Model setup
    FSRModel fsr(sd.getSR(), m_map, conf.P, conf.M, 
                conf.W, id.Inits, sd.getInits());
    
    MatrixXd u_mat; // Optimized actuation
    MatrixXd y_pred;
    sr_solver(T, u_mat, y_pred, fsr, conf, z_min, z_max, sd.getYRef());

    // Serializing: 
    json write_data;
    string scenario = "sr_siso_test";
    string sim_filepath = "../data/simulations/sim_" + scenario + ".json"; 

    SerializeSimulation(write_data, sim_filepath, sys, scenario, sd, id, 
                y_pred, u_mat, z_min, z_max, fsr.getN_CV(), fsr.getN_MV(), T);
}