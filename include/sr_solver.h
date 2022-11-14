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
/**
 * @brief solving the condensed optimalization problem using OSQP-Eigen
 * 
 * @param T mpc horizon
 * @param fsr FSRModel of scenario file
 * @param conf MPCConfig
 */
void sr_solver(const int& T, const FSRModel& fsr, const MPCConfig& conf);

/**
 * @brief Set the Weight Matrices object
 * 
 * @param Q_bar Eigen::SparseMatrix<float> to be filled by output tuning
 * @param R_bar Eigen::SparseMatrix<float> to be filled by change of input tuning
 * @param mpc_config 
 */
void setWeightMatrices(Eigen::SparseMatrix<float>& Q_bar, Eigen::SparseMatrix<float>& R_bar, const MPCConfig& mpc_config);

/**
 * @brief Set the Hessian Matrix object
 * 
 * @param G Positive definite optimization matrix
 * @param theta Step coefficient prediction matrix
 * @param Q_bar Positive definite Eigen::MatrixXf output tuning matrix
 * @param R_bar Positive definite Eigen::MatrixXf change of input tuning matrix
 * @param n_MV number of manipualated variables
 * @param M control horizon
 */
void setHessianMatrix(Eigen::SparseMatrix<float>& G, const Eigen::MatrixXf& theta, const Eigen::MatrixXf& Q_bar, const Eigen::MatrixXf& R_bar, int n_MV, int M); 

/**
 * @brief Calculate K matrix
 * 
 * @param K Eigen::SparseMatrix<float> to be filled 
 * @param M Control horizon
 * @param n_MV number of manupulated variables
 */
void setKmatrix(Eigen::SparseMatrix<float>& K, int M, int n_MV);

/**
 * @brief Calculate K inverse
 * 
 * @param K_inv Eigen::MatrixXf to be filled
 * @param M control horizon
 */
void setKInv(Eigen::MatrixXf& K_inv, int M);

void setConstrainVectors(Eigen::VectorXf& l, Eigen::VectorXf& u, const Eigen::VectorXf& z_max, const Eigen::VectorXf& z_min,
                        const Eigen::VectorXf& lambda, const Eigen::VectorXf& u_N, int M, int n_MV);

void setConstraintMatrix(Eigen::SparseMatrix<float>& A, const FSRModel& fsr, const int& m, const int& n);

void setGamma(Eigen::SparseMatrix<float>& gamma, int M, int n_MV); 

/**
 * @brief Helper function. Implementing block diagonal
 * 
 * @param blk_mat Eigen::SparseMatrix<float> to be block diagonalized
 * @param arg block argument
 * @param count Number of blocks
 */
void blkdiag(Eigen::SparseMatrix<float>& blk_mat, const Eigen::MatrixXf& arg, int count);

#endif // SR_SOLVER_H
