/**
 * @file main.cc
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-26
 * 
 * @copyright  Released under the terms of the BSD 3-Clause License
 * 
 */
#include "simulations.h"
#include <CLI/CLI.hpp>
#include "tests.h"

#include <string>
#include <vector>

using string = std::string;

// Data folder paths
const string SYS_PATH = "../data/systems/";
const string SCE_PATH = "../data/scenarios/";
const string SIM_PATH = "../data/simulations/";

// Common reference values:
const double GAS_RATE_REF = 3800;
const double OIL_RATE_REF = 70;

int main(int argc, char **argv) {

    CLI::App app{"MPC-core"};
    // ---- Parse CLI ---- //

    // Default values:
    int T = 1;
    string sce = "";
    bool new_sim = false; 

    // Add flags: 
    app.add_option("-T", T, "MPC horizon");
    app.add_option("-s", sce, "Scenario name");
    app.add_flag("-n", new_sim, "New simulation");
    CLI11_PARSE(app, argc, argv);

    //TestSimulate();

    // Reference
    std::vector<double> ref_vec{GAS_RATE_REF, OIL_RATE_REF};
    //std::vector<double> ref_vec{1};
    
    // ---- MPC Simulations ---- //
    //OpenLoopFSRM(sce, ref_vec, T);
    MPCSimFSRM(sce, ref_vec, new_sim, T);
}