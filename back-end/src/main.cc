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
#include "LightWeightMPC.h"
#include <CLI/CLI.hpp>

// Test SerializeScenario
#include "IO/serialize.h"
#include "IO/json_specifiers.h"
#include <map>
#include <stdexcept>

#include <string>
#include <vector>

using string = std::string;

// Data paths
const string SYS_PATH = "../data/systems/";
const string SCE_PATH = "../data/scenarios/";
const string SIM_PATH = "../data/simulations/";

const double GAS_RATE_REF = 3800;
const double OIL_RATE_REF = 70;

int main(int argc, char **argv) {
    CLI::App app{"Light Weight MPC"};
    // ---- Parse CLI ---- //
    // Default values
    int T = 1;
    string sce = "";
    bool new_sim = false; 

    app.add_option("-T", T, "MPC horizon");
    app.add_option("-s", sce, "Scenario name");
    app.add_flag("-n", new_sim, "New simulation");
    CLI11_PARSE(app, argc, argv);

    // Reference
    std::vector<double> ref_vec{GAS_RATE_REF, OIL_RATE_REF};
    //const double CHOKE = 100;
    //const double GAS_LIFT = 1000;
    //std::vector<double> ref_vec{CHOKE, GAS_LIFT};

    // Test scenario serializing:
    // string path = SCE_PATH + sce + ".json";
    // string system = "SingleWell";

    // std::map<string, int> mpc_m = {
    //     {kP, 10},
    //     {kM, 5}, 
    //     {kW, 0}
    // };

    // int n_CV = 1;
    // int n_MV = 1;

    // VectorXd Q = VectorXd::Constant(1, 1);
    // VectorXd R = VectorXd::Constant(1, 1);
    // VectorXd Ro { 
    //   {1},     
    //   {1}      
    // };
    // bool bias_update = 0;

    // VectorXd l_du = VectorXd::Constant(1, -1);
    // VectorXd l_u = VectorXd::Constant(1, -1);
    // VectorXd l_y = VectorXd::Constant(1, -1);
    // VectorXd u_du = VectorXd::Constant(1, 1);
    // VectorXd u_u = VectorXd::Constant(1, 1);
    // VectorXd u_y = VectorXd::Constant(1, 1);

    // try {
    //     SerializeScenario(path, sce, system, SYS_PATH, mpc_m, Q, R, Ro, bias_update,
    //     l_du, l_u, l_y, u_du, u_u, u_y, n_CV, n_MV);
    // }
    // catch(std::exception& e) {
    //     std::cout << e.what() << std::endl;
    // }
 
    // ---- Light-weight MPC ---- //
    //OpenLoopSim(sce, ref_vec, T);
    //LightWeightMPC(sce, ref_vec, new_sim, T);
}