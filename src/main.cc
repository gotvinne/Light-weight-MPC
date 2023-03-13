/**
 * @file main.cc
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "LightWeightMPC.h"

#include <string>
#include <vector>
using string = std::string;

const double GAS_RATE_REF = 3800;
const double OIL_RATE_REF = 70;

int main(int argc, char **argv) {
    // Default
    int T = 1;
    string sce = "";
    bool new_sim = false; 

    // Reference
    std::vector<double> ref_vec{GAS_RATE_REF, OIL_RATE_REF};
    //const double CHOKE = 100;
    //const double GAS_LIFT = 1000;
    //std::vector<double> ref_vec{CHOKE, GAS_LIFT};
 
    // ---- Light-weight MPC ---- //
    //OpenLoopSim(sce, ref_vec, T);
    LightWeightMPC(sce, ref_vec, new_sim, T);
    
    return 0;
}