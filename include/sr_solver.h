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
void sr_solver(int T, const FSRModel& fsr, const MPCConfig& conf,
                const VectorXd& z_min, const VectorXd& z_max, VectorXd* y_ref);

/**
 * @brief Set the Weight Matrices object
 * 
 * @param Q_bar Eigen::SparseMatrix<double> to be filled by output tuning
 * @param R_bar Eigen::SparseMatrix<double> to be filled by change of input tuning
 * @param mpc_config 
 */
void setWeightMatrices(SparseXd& Q_bar, SparseXd& R_bar, const MPCConfig& mpc_config);

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
void setHessianMatrix(SparseXd& G, const Eigen::MatrixXd& theta, const MatrixXd& Q_bar, const MatrixXd& R_bar, int n_MV, int M); 

/**
 * @brief Calculate K matrix
 * 
 * @param K Eigen::SparseMatrix<double> to be filled 
 * @param M Control horizon
 * @param n_MV number of manupulated variables
 */
void setKmatrix(SparseXd& K, int M, int n_MV);

/**
 * @brief 
 * 
 * @param K_inv 
 * @param n 
 */
void setKInv(MatrixXd& K_inv, int n);

/**
 * @brief Set the Constraint Vectors object
 * 
 * @param l 
 * @param u 
 * @param z_min 
 * @param z_max 
 * @param fsr 
 * @param m 
 * @param n 
 */
void setConstraintVectors(VectorXd& l, VectorXd& u, const VectorXd& z_min, const VectorXd& z_max,
                        const FSRModel& fsr, int m, int n);

/**
 * @brief Set the Constraint Matrix object
 * 
 * @param A 
 * @param fsr 
 * @param m 
 * @param n 
 */
void setConstraintMatrix(SparseXd& A, const FSRModel& fsr, int m, int n);

/**
 * @brief Set the Gamma object
 * 
 * @param gamma 
 * @param M 
 * @param n_MV 
 */
void setGamma(SparseXd& gamma, int M, int n_MV); 

/**
 * @brief Set the Tau object
 * 
 * @param tau 
 * @param y_ref 
 * @param P 
 * @param W 
 * @param n_CV 
 */
void setTau(VectorXd& tau, VectorXd* y_ref, int P, int W, int n_CV);

/**
 * @brief Set the Gradient Vector object
 * 
 * @param q 
 * @param fsr 
 * @param Q_bar 
 * @param y_ref 
 */
void setGradientVector(VectorXd& q, const FSRModel& fsr, const SparseXd& Q_bar,
                        VectorXd* y_ref);

/**
 * @brief Helper function. Implementing block diagonal
 * 
 * @param blk_mat Eigen::SparseMatrix<double> to be block diagonalized
 * @param arg block argument
 * @param count Number of blocks
 */
void blkdiag(SparseXd& blk_mat, const MatrixXd& arg, int count);

#endif // SR_SOLVER_H
