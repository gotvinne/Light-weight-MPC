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
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

int main() {

    // Testing step model:
    //float k = 5;
    //float tau = 15;
    //float theta = 3;
    //int N = 80;

    const int T = 80; // MPC horizon. 
    string sys_filepath = "../data/systems/sr_siso_test.json";
    string sce_filepath = "../data/scenarios/siso_test.json";
    json sys_data = ReadJson(sys_filepath);
    json sce_data = ReadJson(sce_filepath);

    // Parse system
    std::map<string, int> m_param; //Only used to create FSR
    CVData sd;
    MVData id;
    ParseSystemData(sys_data, m_param, sd, id, T);
    
    // Parse scenario:
    string system; 
    MPCConfig conf; //Default initializer
    VectorXd z_max; // These constraints can be used directly in solver
    VectorXd z_min; 
    ParseScenarioData(sce_data, system, conf, z_max, z_min, m_param[kN_CV], m_param[kN_MV]);
    
    FSRModel fsr(sd.getSR(), m_param, conf.P, conf.M, conf.W, id.Inits, sd.getInits());

    MatrixXd u_mat; // Optimized actuation
    MatrixXd y_pred;
    sr_solver(T, u_mat, y_pred, fsr, conf, z_min, z_max, sd.getYRef());

    // Formatting: 
    json output_data;
    string scenario = "sr_siso_test";
    string sim_filepath = "../data/simulations/sim_" + scenario + ".json"; 

    FormatSimData(output_data, sim_filepath, scenario, T, fsr.getN_CV(), fsr.getN_MV());
    FormatSimCV(output_data, sd, y_pred, fsr.getN_CV());
    FormatSimMV(output_data, id, u_mat, fsr.getN_MV());
    WriteJson(output_data, sim_filepath);

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