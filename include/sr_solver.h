/**
 * @file sr_solver.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef SR_SOLVER_H
#define SR_SOLVER_H

#include <Eigen/Dense>
#include "data_objects.h"
#include "FSRModel.h"

/**
 * @brief 
 * 
 */
void sr_solver(const int& T, const FSRModel& fsr, const MPCConfig& conf);
void setWeightMatrices(Eigen::MatrixXf& Q_bar, Eigen::MatrixXf& R_bar, const MPCConfig& mpc_config);
void setHessianMatrix(Eigen::MatrixXf& G, const Eigen::MatrixXf& theta, const Eigen::MatrixXf& Q_bar, const Eigen::MatrixXf& R_bar, int n_MV, int M); 

void setKmatrix(Eigen::MatrixXf& K, int M, int n_MV);
void setKInv(Eigen::MatrixXf& K_inv, int M);

void setConstrainVectors(Eigen::VectorXf& l, Eigen::VectorXf& u, const Eigen::VectorXf& z_max, const Eigen::VectorXf& z_min);
void setConstraintMatrix(Eigen::MatrixXf& A, const FSRModel& fsr, const int& m, const int& n);

void blkdiag(Eigen::MatrixXf& blk_mat, const Eigen::MatrixXf& arg, int count);



#endif // SR_SOLVER_H
