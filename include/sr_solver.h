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
void sr_solver(const int& T, const FSRModel& fsr, const MPCConfig& conf,
                const Eigen::VectorXd& z_min, const Eigen::VectorXd& z_max, Eigen::VectorXd* y_ref);

/**
 * @brief Set the Weight Matrices object
 * 
 * @param Q_bar Eigen::SparseMatrix<double> to be filled by output tuning
 * @param R_bar Eigen::SparseMatrix<double> to be filled by change of input tuning
 * @param mpc_config 
 */
void setWeightMatrices(Eigen::SparseMatrix<double>& Q_bar, Eigen::SparseMatrix<double>& R_bar, const MPCConfig& mpc_config);

/**
 * @brief Set the Hessian Matrix object
 * 
 * @param G Positive definite optimization matrix
 * @param theta Step coefficient prediction matrix
 * @param Q_bar Positive definite Eigen::MatrixXd output tuning matrix
 * @param R_bar Positive definite Eigen::MatrixXd change of input tuning matrix
 * @param n_MV number of manipualated variables
 * @param M control horizon
 */
void setHessianMatrix(Eigen::SparseMatrix<double>& G, const Eigen::MatrixXd& theta, const Eigen::MatrixXd& Q_bar, const Eigen::MatrixXd& R_bar, int n_MV, int M); 

/**
 * @brief Calculate K matrix
 * 
 * @param K Eigen::SparseMatrix<double> to be filled 
 * @param M Control horizon
 * @param n_MV number of manupulated variables
 */
void setKmatrix(Eigen::SparseMatrix<double>& K, int M, int n_MV);

/**
 * @brief Calculate K inverse
 * 
 * @param K_inv Eigen::MatrixXd to be filled
 * @param M control horizon
 */
void setKInv(Eigen::MatrixXd& K_inv, int M);

void setConstrainVectors(Eigen::VectorXd& l, Eigen::VectorXd& u, const Eigen::VectorXd& z_max, const Eigen::VectorXd& z_min,
                        const Eigen::VectorXd& lambda, const Eigen::VectorXd& u_k, const int& M, const int& n_MV, const int& m);

void setConstraintMatrix(Eigen::SparseMatrix<double>& A, const FSRModel& fsr, const int& m, const int& n);

void setGamma(Eigen::SparseMatrix<double>& gamma, int M, int n_MV); 

void setTau(Eigen::VectorXd& tau, Eigen::VectorXd* y_ref, const int& P, const int& W, const int& n_CV);

void setGradientVector(Eigen::VectorXd& q, const Eigen::VectorXd& lambda, const Eigen::MatrixXd& theta, const Eigen::SparseMatrix<double>& Q_bar,
                        Eigen::VectorXd* y_ref, const int& P, const int& W, const int& n_CV);

/**
 * @brief Helper function. Implementing block diagonal
 * 
 * @param blk_mat Eigen::SparseMatrix<double> to be block diagonalized
 * @param arg block argument
 * @param count Number of blocks
 */
void blkdiag(Eigen::SparseMatrix<double>& blk_mat, const Eigen::MatrixXd& arg, int count);

#endif // SR_SOLVER_H
