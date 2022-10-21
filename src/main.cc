// Copyright (C) Geir Ola Tvinnereim 2022

#include <iostream>
#include <string>

#include "formatting.h"
#include "step_coefficients.h"
#include "sr_solver.h"
#include "parse.h"

#include <array>
#include <string>

int main() {

    // Testing step model:
    float k = 5;
    float tau = 15;
    float theta = 3;
    int N = 80;

    const int T = 50; 

    std::string filepath = "../data/systems/sr_siso_test.json";
    json data = read_json(filepath);
    std::array<int, 3> model = system_model_data(data);
    
    print_container(model);

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