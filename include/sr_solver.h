/**
 * @file sr_solver.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef SR_SOLVER_H
#define SR_SOLVER_H

#include <string>
#include <map>

#include <Eigen/Dense>
#include "parse.h"


/**
 * @brief 
 * 
 */
void sr_solver(const int& T, std::map<std::string,int>& model_param);

void setWeightMatrices(Eigen::MatrixXf& Q_bar, Eigen::MatrixXf& R_bar, const MPCConfig& mpc_config);

void setThetaMatrix(Eigen::MatrixXf& theta, const CVData& cv_data, const MPCConfig& mpc_config);

void setPhiMatrix(Eigen::MatrixXf& phi, const CVData& cv_data, const MPCConfig& mpc_config);

void setHessianMatrix(Eigen::MatrixXf& hessian, const Eigen::MatrixXf& theta, const Eigen::MatrixXf& Q_bar, const Eigen::MatrixXf& R_bar, int n_MV, int M); 




#endif // SR_SOLVER_H
