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

int main(int argc, char **argv) {
    CLI::App app{"MPC-core"};
    // ---- Parse CLI ---- //
    // Default values:
    int T = 1;
    string sce = "";
    string ref_vec = "";
    bool new_sim = false; 

    // Add flags: 
    app.add_option("-T", T, "MPC horizon");
    app.add_option("-s", sce, "Scenario name");
    app.add_option("-r", ref_vec, "Reference vector"); 
    app.add_flag("-n", new_sim, "New simulation");
    CLI11_PARSE(app, argc, argv);
    
    // ---- MPC Simulations ---- //
    MPCSimFSRM(sce, ref_vec, new_sim, T);
}