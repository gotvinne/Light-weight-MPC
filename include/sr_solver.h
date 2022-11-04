/**
 * @file sr_solver.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef SR_SOLVER_H
#define SR_SOLVER_H

#include <Eigen/Dense>
#include "data_structs.h"
#include "FSRModel.h"


/**
 * @brief 
 * 
 */
void sr_solver(const int& T, const FSRModel& fsr);

void setWeightMatrices(Eigen::MatrixXf& Q_bar, Eigen::MatrixXf& R_bar, const MPCConfig& mpc_config);

void setHessianMatrix(Eigen::MatrixXf& hessian, const Eigen::MatrixXf& theta, const Eigen::MatrixXf& Q_bar, const Eigen::MatrixXf& R_bar, int n_MV, int M); 




#endif // SR_SOLVER_H
