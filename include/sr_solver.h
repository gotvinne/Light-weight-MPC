/**
 * @file sr_solver.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef SR_SOLVER_H
#define SR_SOLVER_H

#include "data_objects.h"
#include "FSRModel.h"

#include <Eigen/Eigen>

using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using SparseXd = Eigen::SparseMatrix<double>;

/**
 * @brief solving the condensed optimalization problem using OSQP-Eigen
 * 
 * @param T mpc horizon
 * @param fsr FSRModel of scenario file
 * @param conf MPCConfig
 */
void sr_solver(int T, MatrixXd& u_mat, MatrixXd& y_pred, FSRModel& fsr, const MPCConfig& conf,
            const VectorXd& z_min, const VectorXd& z_max, VectorXd* y_ref);

#endif // SR_SOLVER_H
