/**
 * @file sr_solver.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */
#ifndef SR_SOLVER_H
#define SR_SOLVER_H

#include "IO/data_objects.h"
#include "model/FSRModel.h"

#include <Eigen/Eigen>
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using SparseXd = Eigen::SparseMatrix<double>;

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
void sr_solver(int T, MatrixXd& u_mat, MatrixXd& y_pred, FSRModel& fsr, const MPCConfig& conf,
            const VectorXd& z_min, const VectorXd& z_max, VectorXd* y_ref);

#endif // SR_SOLVER_H
