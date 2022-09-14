// Copyright (C) Geir Ola Tvinnereim 2022

#include <iostream>
#include <string>

#include "../include/formatting.h"

int main() {

    json data = parse("../scenarios/test.json");
    std::cout << data.dump() << std::endl;

    // Flow: 
    std::string scenario {"../scenarios/siso_test.json"};

    // Read scenario and check if it is valid
    // Simulate scenario returning simulation

    // Write simulation to an output file


    // Evt. 
    // Call python script to read data and plot simulation. 
    return 0;
}