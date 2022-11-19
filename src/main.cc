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
#include "data_objects.h"

#include <map>
#include <iostream>
#include <string>

#include "nlohmann/json.hpp"
#include <Eigen/Dense>
using json = nlohmann::json; 

int main() {

    // Testing step model:
    //float k = 5;
    //float tau = 15;
    //float theta = 3;
    //int N = 80;

    const int T = 80; // MPC horizon. 
    std::string sys_filepath = "../data/systems/sr_siso_test.json";
    std::string sce_filepath = "../data/scenarios/siso_test.json";
    json sys_data = ReadJson(sys_filepath);
    json sce_data = ReadJson(sce_filepath);

    // Parse system
    std::map<std::string, int> m_param; //Only used to create FSR
    CVData sd;
    MVData id;
    ParseSystemData(sys_data, m_param, sd, id, T);
    
    // Parse scenario:
    std::string system; 
    MPCConfig conf; //Default initializer
    Eigen::VectorXd z_max; // These constraints can be used directly in solver
    Eigen::VectorXd z_min; 
    ParseScenarioData(sce_data, system, conf, z_max, z_min, m_param[kN_CV], m_param[kN_MV]);
    
    FSRModel fsr(sd.getSR(), m_param[kN_CV], m_param[kN_MV], m_param[kN], conf.P, conf.M,
                conf.W, id.Inits, sd.getInits());
    sr_solver(T, fsr, conf, z_min, z_max, sd.getYRef());

    // Eigen::MatrixXf dt_opt; // Optimal actuation 
    Eigen::MatrixXf du; // n_CV x T

    // Formatting: 
    json output_data;
    std::string scenario = "sr_siso_test";
    std::string sim_filepath = "../data/scenario/sim_" + scenario; 

    //FormatSimData(output_data, sim_filepath, scenario, T, fsr.getN_CV(), fsr.getN_MV());
    //FormatSimCV(output_data, sd, fsr.getN_CV());
    //FormatSimMV(output_data, id, fsr.getN_MV());
    //WriteJson(output_data, sim_filepath);

    // // Flow: 
    // 1)
    // // Read scenario and check if it is valid **

    // 2)
    // // Simulate scenario closed loop using reference model returning simulation

    // 3)
    // // Write simulation to an output file

    // 4)
    // // Call python script to read data and plot simulation. **
  
    return 0;
}