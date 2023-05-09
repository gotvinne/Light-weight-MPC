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

#include <vector>
#include <string>
#include <Eigen/Dense>

using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

/**
 * @brief MPC simulation of FSR model, can also do simulations further. Scenario file: sce_sys.json
 * 
 * @param sys System to be simulated
 * @param ref_vec vector holding reference values
 * @param new_sim New simulation or simulate further
 * @param T MPC horizon
 */
void MPCSimFSRM(const string& sys, const string& ref_vec, bool new_sim, int T);

/**
 * @brief Open loop simulation of FSR model
 * 
 * @param sys System name
 * @param ref_vec Reference values
 * @param T Simulations steps
 */
void OpenLoopFSRM(const string& sys, const std::vector<double>& ref_vec, int T);

#endif // SIMULATIONS_H
