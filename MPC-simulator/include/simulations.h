/**
 * @file simulations.h
 * @author Geir Ola Tvinnereim
 * @brief Script for simulation functionalities
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright  Released under the terms of the BSD 3-Clause License
 * 
 */
#ifndef SIMULATIONS_H
#define SIMULATIONS_H

#include <string>
using string = std::string;

enum class MPC_FSRM_Simulation {
    CONDENSED, // Condensed controller with slack constraints
    CONDENSED_W, // Condensed controller with slack constraints, reduced cost due to W != 0
    CONDENSED_WoSlack, // Condensed controller without slack constraints
    CONDENSED_W_WoSlack // Condensed controller without slack constraints, reduced cost due to W != 0
};

/**
 * @brief MPC simulation of FSR model, can also do simulations further. Scenario file: sce_sys.json
 * 
 * @param sys System to be simulated
 * @param ref_vec vector holding reference values
 * @param new_sim New simulation or simulate further
 * @param T MPC horizon
 */
void MPCSimFSRM(const string& sys, const string& ref_vec, bool new_sim, int T);

#endif // SIMULATIONS_H
