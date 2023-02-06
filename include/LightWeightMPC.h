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

#include <map>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include <Eigen/Dense>
using json = nlohmann::json; 
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

void LightWeightMPC(const string& sce, int T);

/**
 * @brief Solving the condensed optimalization problem using OSQP-Eigen
 * 
 * @param T MPC horizon
 * @param u_mat Optimized u, filled by reference
 * @param y_pred Predicted y, filled by reference
 * @param fsr FSRModel, finite step response model 
 * @param conf MPC configuration
 * @param z_min lower constraint vector
 * @param z_max upper constraint vector
 * @param y_ref Output reference data
 */
void SRSolver(int T, MatrixXd& u_mat, MatrixXd& y_pred, FSRModel& fsr, const MPCConfig& conf, const VectorXd& z_min, 
             const VectorXd& z_max, VectorXd* y_ref);

#endif // LIGHTWEIGHT_MPC_H