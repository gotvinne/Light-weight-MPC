/**
 * @file LightWeightMPC.h
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LIGHTWEIGHTMPC_H
#define LIGHTWEIGHTMPC_H

#include <vector>
#include <string>

#include <nlohmann/json.hpp>
#include <Eigen/Dense>
using json = nlohmann::json; 
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

/**
 * @brief Simulation software function
 * 
 * @param sce [std::string] Scenario to be simulated 
 * @param ref_vec [std::vector] vector holding reference values
 * @param new_sim [bool]
 * @param T [int] MPC horizon
 */
void LightWeightMPC(const string& sce, const std::vector<double>& ref_vec, bool new_sim, int T);

void OpenLoopSim(const string& sce, const std::vector<double>& ref_vec, bool new_sim, int T);

#endif // LIGHTWEIGHTMPC_H
