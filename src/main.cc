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
#include "IO/update.h"

#include <CLI/CLI.hpp>

#include <string>
using string = std::string;

int main(int argc, char **argv) {
    CLI::App app{"Light Weight MPC"};
    // ---- Parse CLI ---- //

    // Default
    int T = 1;
    string sce = "";

    app.add_option("-T", T, "MPC horizon");
    app.add_option("-s", sce, "Scenario name");
    CLI11_PARSE(app, argc, argv);
 
    // ---- Light-weight MPC ---- //
    LightWeightMPC(sce, T);

    //double value = 1;
    //UpdateReference("../data/systems/sr_SingleWell.json", value, T);
    return 0;
}