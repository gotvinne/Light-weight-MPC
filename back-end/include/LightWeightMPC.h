/**
 * @file LightWeightMPC.h
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright  Released under the terms of the BSD 3-Clause License
 * 
 */
#ifndef LIGHTWEIGHTMPC_H
#define LIGHTWEIGHTMPC_H

#include <vector>
#include <string>
#include <Eigen/Dense>

using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

/**
 * @brief Allocates and initialises reference
 * 
 * @param ref_vec [std::vector] Vector holding reference values
 * @param T [int] MPC horizon
 * @param P [int] Prediction horizon
 */
VectorXd* AllocateConstReference(const std::vector<double>& ref_vec, int T, int P);
    
/**
 * @brief Simulation software function
 * 
 * @param sce [std::string] Scenario to be simulated 
 * @param ref_vec [std::vector] vector holding reference values
 * @param new_sim [bool]
 * @param T [int] MPC horizon
 */
void LightWeightMPC(const string& sce, const std::vector<double>& ref_vec, bool new_sim, int T);

/**
 * @brief 
 * 
 * @param sce 
 * @param ref_vec 
 * @param T 
 */
void OpenLoopSim(const string& sce, const std::vector<double>& ref_vec, int T);

#endif // LIGHTWEIGHTMPC_H
