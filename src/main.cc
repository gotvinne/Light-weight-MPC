/**
 * @file main.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "formatting.h"
#include "step_response_model.h"
#include "sr_solver.h"
#include "parse.h"
#include "FSRModel.h"
#include "data_objects.h"

#include <map>
#include <iostream>
#include <string>

#include <CLI/CLI.hpp>
#include "nlohmann/json.hpp"
#include <Eigen/Dense>
using json = nlohmann::json; 
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

int main(int argc, char **argv) {
    CLI::App app{"Light Weight MPC"};
    // Parse CLI
    int T = 1;
    app.add_option("-T", T, "MPC horizon");
    CLI11_PARSE(app, argc, argv);

    // Testing step model:
    //float k = 5;
    //float tau = 15;
    //float theta = 3;
    //int N = 80;
 
    string sys_path = "../data/systems/sr_siso_test.json";
    string sce_path = "../data/scenarios/siso_test.json";
    // System variables
    std::map<string, int> m_map;
    CVData sd;
    MVData id;
    // Scenario variables:
    string sys; 
    MPCConfig conf; //Default initializer
    VectorXd z_max;
    VectorXd z_min; 
    Parse(sys_path, sce_path, m_map, sd, id, sys, 
            conf, z_min, z_max, T);

    FSRModel fsr(sd.getSR(), m_map, conf.P, conf.M, 
                conf.W, id.Inits, sd.getInits());
    //fsr.PrintPsi();
    MatrixXd u_mat; // Optimized actuation
    MatrixXd y_pred;
    sr_solver(T, u_mat, y_pred, fsr, conf, z_min, z_max, sd.getYRef());

    // Formatting: 
    json write_data;
    string scenario = "sr_siso_test";
    string sim_filepath = "../data/simulations/sim_" + scenario + ".json"; 

    //FormatSimData(write_data, sim_filepath, scenario, T, fsr.getN_CV(), fsr.getN_MV());
    //FormatSimCV(write_data, sd, y_pred, fsr.getN_CV());
    //FormatSimMV(write_data, id, u_mat, fsr.getN_MV());
    //WriteJson(write_data, sim_filepath);
    FormatScenario(write_data, sim_filepath, sys, scenario, sd, id, 
                y_pred, u_mat, fsr.getN_CV(), fsr.getN_MV(), T);

    // // Flow: 
    // 1)
    // // Read scenario and check if it is valid **

    // 2)
    // // Simulate scenario closed loop using reference model returning simulation
        // SISO Simulation *
        // Reference model
        // MIMO Simulation
        // Output feedback
        // Slack variables

    // 3)
    // // Write simulation to an output file 

    // 4)
    // // Call python script to read data and plot simulation. **
  
    return 0;
}