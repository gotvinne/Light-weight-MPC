/**
 * @file main.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "lightweight_MPC.h"

#include "IO/update.h"

#include <CLI/CLI.hpp>

int main(int argc, char **argv) {
    CLI::App app{"Light Weight MPC"};
    // Parse CLI
    int T = 1;
    app.add_option("-T", T, "MPC horizon");
    CLI11_PARSE(app, argc, argv);
 
    LightWeightMPC(T);
    //double value = 1;
    //UpdateReference("../data/systems/sr_SingleWell.json", value, T);


    return 0;
}