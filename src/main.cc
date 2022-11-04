/**
 * @file main.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "formatting.h"
#include "step_response_model.h"
#include "json_specifiers.h"
#include "sr_solver.h"
#include "parse.h"
#include "FSRModel.h"
#include "data_structs.h"

#include <map>
#include <iostream>
#include <string>

#include "nlohmann/json.hpp"
#include <Eigen/Dense>

using json = nlohmann::json; 

int main() {

    // Testing step model:
    float k = 5;
    float tau = 15;
    float theta = 3;
    int N = 80;

    const int T = 80; // THIS is an input variable, MPC horizon. 
    std::string sys_filepath = "../data/systems/sr_siso_test.json";
    std::string sce_filepath = "../data/scenarios/siso_test.json";
    json sys_data = ReadJson(sys_filepath);
    json sce_data = ReadJson(sce_filepath);

    // Parse system
    std::map<std::string, int> m_param;
    CVData sd;
    MVData id;
    ParseSystemData(sys_data, m_param, sd, id, T);
    
    // Parse scenario:
    std::string system; 
    MPCConfig conf; //Default initializer
    Eigen::VectorXf z_max; // These constraints can be used directly in solver
    Eigen::VectorXf z_min; 
    ParseScenarioData(sce_data, system, conf, z_max, z_min, m_param[kN_CV], m_param[kN_MV]);
    
    //FSRModel S(sd.S, m_param[kN_CV], m_param[kN_MV], m_param[kN], conf.P, conf.M, conf.W);
    //S.PrintSR();
    //S.PrintPhi();

    // // Defining MPC matrices
    // Eigen::MatrixXf Q_bar; 
    // Eigen::MatrixXf R_bar; 
    // setWeightMatrices(Q_bar, R_bar, mpc_config);

    // Eigen::MatrixXf dt_opt; // Optimal actuation 
    

    // // Flow: 
    // 1)
    // // Read scenario and check if it is valid

    // 2)
    // // Simulate scenario closed loop using reference model returning simulation

    // 3)
    // // Write simulation to an output file

    // 4)
    // // Call python script to read data and plot simulation. 
  
    return 0;
}