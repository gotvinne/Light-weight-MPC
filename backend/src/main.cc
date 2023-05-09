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

#include <string>
using string = std::string;

int main(int argc, char **argv) {
    CLI::App app{"MPC-core"};
    // ---- Parse CLI ---- //
    // Default values:
    int T = 1;
    string sys = "";
    string ref_str = "";
    bool new_sim = false; 

    // Add flags: 
    app.add_option("-T", T, "MPC horizon");
    app.add_option("-s", sys, "System name");
    app.add_option("-r", ref_str, "Reference vector"); 
    app.add_flag("-n", new_sim, "New simulation");
    CLI11_PARSE(app, argc, argv);

    // NB! The system file sys.json must be located inside data/systems folder
    // The corresponding sce_sys.json file will be used as scenario. This must be found in data/scenarios
    
    // ---- MPC Simulations ---- //
    //OpenLoopFSRM(sys, ref_str, T);
    MPCSimFSRM(sys, ref_str, new_sim, T);
}