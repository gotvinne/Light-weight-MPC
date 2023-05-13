/**
 * @file solvers.h
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Released under the terms of the BSD 3-Clause License
 * 
 */
#ifndef SOLVERS_H
#define SOLVERS_H

#include "model/FSRModel.h"
#include "IO/data_objects.h"

#include <Eigen/Eigen>

using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
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
 * @param ref Output reference data
 */
void SRSolver(int T, MatrixXd& u_mat, MatrixXd& y_pred, FSRModel& fsr, const MPCConfig& conf, const VectorXd& z_min, 
             const VectorXd& z_max, const MatrixXd& ref);

/**
 * @brief 
 * 
 * @param T 
 * @param u_mat 
 * @param y_pred 
 * @param fsr_sim 
 * @param fsr_cost 
 * @param conf 
 * @param z_min 
 * @param z_max 
 * @param ref 
 */
void SRSolver(int T, MatrixXd& u_mat, MatrixXd& y_pred, FSRModel& fsr_sim, FSRModel& fsr_cost, const MPCConfig& conf, const VectorXd& z_min, 
             const VectorXd& z_max, const MatrixXd& ref);

#endif // SOLVERS_H