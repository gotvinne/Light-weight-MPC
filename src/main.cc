// Copyright (C) Geir Ola Tvinnereim 2022

#include <iostream>
#include <string>

#include "../include/formatting.h"
#include "../include/step_coefficients.h"
#include "../include/example_mpc.h"

int main() {

    // Testing step model:
    float k = 5;
    float tau = 15;
    float theta = 3;
    int N = 80;

    solver();

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