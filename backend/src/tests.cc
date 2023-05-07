/**
 * @file tests.cc
 * @author Geir Ola Tvinnereim
 * @brief Scripts defining tests for TDD
 * @version 0.1
 * @date 2023-05-03
 * 
 * @copyright Released under the terms of the BSD 3-Clause License
 * 
 */

#include "tests.h"
#include "IO/json_specifiers.h"
#include "IO/serialize.h"
#include "IO/parse.h"

#include "wasm/wasm.h"

#include <iostream>
#include <vector>
#include <map>

#include <Eigen/Dense>
#include <nlohmann/json.hpp>

using VectorXd = Eigen::VectorXd;
using json = nlohmann::json; 
using string = std::string;

void TestSerializeScenario(const string& sce, const string& sys, const string& SCE_PATH, const string& SYS_PATH) {
    const double CHOKE = 100;
    const double GAS_LIFT = 1000;
    std::vector<double> ref_vec{CHOKE, GAS_LIFT};

    //Test scenario serializing:
    string path = SCE_PATH + sce + ".json";

    std::map<string, int> mpc_m = {
        {kP, 10},
        {kM, 5}, 
        {kW, 0}
    };

    int n_CV = 1;
    int n_MV = 1;

    VectorXd Q = VectorXd::Constant(1, 1);
    VectorXd R = VectorXd::Constant(1, 1);
    VectorXd Ro { 
      {1},     
      {1}      
    };
    bool bias_update = 0;

    VectorXd l_du = VectorXd::Constant(1, -1);
    VectorXd l_u = VectorXd::Constant(1, -1);
    VectorXd l_y = VectorXd::Constant(1, -1);
    VectorXd u_du = VectorXd::Constant(1, 1);
    VectorXd u_u = VectorXd::Constant(1, 1);
    VectorXd u_y = VectorXd::Constant(1, 1);

    try {
        SerializeScenario(path, sce, sys, SYS_PATH, mpc_m, Q, R, Ro, bias_update,
        l_du, l_u, l_y, u_du, u_u, u_y, n_CV, n_MV);
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void TestSimulate() {
    // TEST PARAMETERS //
    string sys = "SingleWell";

    json ref = json::object();
    ref["ref"] = {3800, 70};
    /////////////////
    string ref_str = to_string(ref);

    string sce = "test";
    int T = 100;

    string sce_filepath = "../data/scenarios/sce_" + sys + ".json";
    json scenario = ReadJson(sce_filepath);
    string sce_file = to_string(scenario);

    string sys_filepath = "../data/systems/" + sys + ".json";
    json system = ReadJson(sys_filepath);
    string sys_file = to_string(system);

    string data = simulate(sce_file, sys_file, sce, ref_str, T);
    
    std::cout << data << std::endl;
}