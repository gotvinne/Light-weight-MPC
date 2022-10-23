// Copyright (C) Geir Ola Tvinnereim 2022

#include <iostream>
#include <string>

#include "formatting.h"
#include "step_coefficients.h"
#include "sr_solver.h"
#include "parse.h"

#include <array>
#include <string>

#include <typeinfo> // For type checking
int main() {

    // Testing step model:
    float k = 5;
    float tau = 15;
    float theta = 3;
    int N = 80;

    const int T = 50; // THIS is an input variable, MPC horizon. 

    std::string system_filepath = "../data/systems/sr_siso_test.json";
    std::string scenario_filepath = "../data/scenarios/siso_test.json";
    json system_data = ReadJson(system_filepath);
    json scenario_data = ReadJson(scenario_filepath);

    std::cout << typeid(scenario_data[kC]).name() << std::endl;
    
    


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

    sr_solver();

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