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
#include <CLI/CLI.hpp>

#include <string>
#include <vector>
using string = std::string;

const double GAS_RATE_REF = 11500;
const double OIL_RATE_REF = 280;

int main(int argc, char **argv) {
    CLI::App app{"Light Weight MPC"};
    // ---- Parse CLI ---- //

    // Default
    int T = 1;
    string sce = "";
    bool new_sim = false; 

    app.add_option("-T", T, "MPC horizon");
    app.add_option("-s", sce, "Scenario name");
    app.add_flag("-n", new_sim, "New simulation");
    CLI11_PARSE(app, argc, argv);

    // Reference
    //std::vector<double> ref_vec{GAS_RATE_REF, OIL_RATE_REF};
    const double CHOKE = 100;
    const double GAS_LIFT = 0;
    std::vector<double> ref_vec{CHOKE, GAS_LIFT};
 
    // ---- Light-weight MPC ---- //
    OpenLoopSim(sce, ref_vec, T);
    //LightWeightMPC(sce, ref_vec, new_sim, T);
    
    return 0;
}