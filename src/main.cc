/**
 * @file main.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include <iostream>
#include <string>

#include "formatting.h"
#include "step_coefficients.h"
#include "sr_solver.h"
#include "parse.h"

#include <map>
#include <Eigen/Dense>
#include "nlohmann/json.hpp"

#include <typeinfo> // For type checking

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
    std::map<std::string, int> model_param;
    ModelData(sys_data, model_param);
    
    json cv_data = sys_data.at(kCV);
    json mv_data = sys_data.at(kMV);

    InputData id(mv_data, model_param[kN_MV], T);
    //StateData sd(cv_data, model_param[kN_MV], model_param[kN_CV], model_param[kN]);

    
    //int n_states = mv_data.size();
    //for (int states = 0; states < n_states; states++) {
    //    json data = mv_data.at(states);
    //    std::cout << data.at(kU) << std::endl; 
    //}

    // Parse Scenario:
    std::string system; 
    MPCConfig mpc_config; //Default initializer
    Eigen::ArrayXf upper; 
    Eigen::ArrayXf lower; 

    //ParseScenarioData(sce_data, system, mpc_config, upper, lower, 1, 1);
    //sr_solver();
    
    // // Flow: 
    // 1)
    // std::string scenario {"../scenarios/siso_test.json"};
    // // Read scenario and check if it is valid

    // 2)
    // // Simulate scenario returning simulation

    // 3)
    // // Write simulation to an output file

    // 4)
    // // Call python script to read data and plot simulation. 
  
    return 0;
}