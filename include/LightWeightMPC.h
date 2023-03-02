/**
 * @file lightweight_MPC.h
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LIGHTWEIGHT_MPC_H
#define LIGHTWEIGHT_MPC_H

#include "IO/serialize.h"
#include "IO/parse.h"
#include "IO/data_objects.h"
#include "model/step_response_model.h"
#include "model/FSRModel.h"

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

#endif // LIGHTWEIGHT_MPC_H
