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

#include <array>
#include <Eigen/Dense>

#include <typeinfo> // For type checking
int main() {

    // Testing step model:
    float k = 5;
    float tau = 15;
    float theta = 3;
    int N = 80;

    const int T = 50; // THIS is an input variable, MPC horizon. 

    std::string sys_filepath = "../data/systems/sr_siso_test.json";
    std::string sce_filepath = "../data/scenarios/siso_test.json";

    json sys_data = ReadJson(sys_filepath);
    json sce_data = ReadJson(sce_filepath);

    // Parse Scenario:
    std::string system; 
    MPCConfig mpc_config; //Default initializer
    Eigen::ArrayXf upper; 
    Eigen::ArrayXf lower; 

    ParseScenarioData(sce_data, system, mpc_config, upper, lower, 1, 1);

    std::cout << system << std::endl; 
    std::cout << mpc_config.Q << std::endl; 
    std::cout << upper << std::endl;

    //ParseScenarioData(sce_data, mpc_config, upper)
    

    /**
     Read
     name
     T
     scenario
     system

     Q_
     R_

     W 
     P
     M

     S

     constraints

    **/ 

    //sr_solver();

    //std::vector<float> vec = step_coefficients(k, tau, theta, N);
    //print_coefficients(vec);
    
    // json data = parse("../scenarios/test.json");
    // std::cout << data.dump() << std::endl;

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