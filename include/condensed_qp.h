/**
 * @file condensed_qp.
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef CONDENSED_QP_H
#define CONDENSED_QP_H

#include "data_objects.h"
#include "FSRModel.h"

#include <string>

#include <Eigen/Eigen>
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using SparseXd = Eigen::SparseMatrix<double>;

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
 * @param Q_bar Positive definite Eigen::MatrixXd output tuning matrix
 * @param R_bar Positive definite Eigen::MatrixXd change of input tuning matrix
 * @param fsr 
 */
void setHessianMatrix(SparseXd& G, const SparseXd& Q_bar, const SparseXd& R_bar, const FSRModel& fsr); 

/**
 * @brief Calculate K matrix
 * 
 * @param K Eigen::SparseMatrix<double> to be filled 
 * @param M Control horizon
 * @param n_MV number of manupulated variables
 */
void setKmatrix(SparseXd& K, int M, int n_MV);

/**
 * @brief Calculate K inv matrix, this is a lower triangular matrix
 * 
 * @param K_inv Eigen::MatrixXd passed by reference
 * @param n size of K_inv
 */
void setKInv(MatrixXd& K_inv, int n);

/**
 * @brief Set the Constraint Vectors object, NB! Dynamic output
 * 
 * @param l Eigen::VectorXd lower constraint vector 
 * @param u Eigen::VectorXd upper constraint vector
 * @param z_min lower constraints, du, u, y
 * @param z_max upper constraints, du, u, y
 * @param fsr Finite step response model
 * @param m Number of constraints
 * @param n Number of optimization variables
 */
void setConstraintVectors(VectorXd& l, VectorXd& u, const VectorXd& z_min, const VectorXd& z_max,
                        FSRModel& fsr, int m, int n);

/**
 * @brief Set the Constraint Matrix object
 * 
 * @param A Eigen::SparseMatrix<double>
 * @param fsr Finite step response model
 * @param m Number of constraints
 * @param n Number of optimization variables
 */
void setConstraintMatrix(SparseXd& A, const FSRModel& fsr, int m, int n);

/**
 * @brief Set the Gamma object
 * 
 * @param gamma 
 * @param M Control horizon
 * @param n_MV number of manipulated variables
 */
void setGamma(SparseXd& gamma, int M, int n_MV); 

/**
 * @brief Set the Omega U object
 * 
 * @param Omega_u
 * @param M 
 * @param n_MV 
 */
void setOmegaU(SparseXd& omega, int M, int n_MV);

/**
 * @brief Set the Omega y object
 * 
 * @param Omega_y
 * @param P
 * @param n_CV 
 */
void setOmegaY(SparseXd& omega, int P, int n_CV);

/**
 * @brief Set the Tau object
 * 
 * @param tau 
 * @param y_ref 
 * @param P Prediction horizon
 * @param W Time delay horizon
 * @param n_CV Number of controlled variables
 */
void setTau(VectorXd& tau, VectorXd* y_ref, int P, int W, int n_CV);

/**
 * @brief Set the Gradient Vector object, NB! Dynamic output
 * 
 * @param q 
 * @param fsr Finite step response model
 * @param Q_bar output tuning
 * @param y_ref 
 */
void setGradientVector(VectorXd& q, FSRModel& fsr, const SparseXd& Q_bar,
                        VectorXd* y_ref);

/**
 * @brief Helper function. Implementing block diagonal
 * 
 * @param blk_mat Eigen::SparseMatrix<double> to be block diagonalized
 * @param arg block argument
 * @param count Number of blocks
 */
void blkdiag(SparseXd& blk_mat, const MatrixXd& arg, int count);

#endif // CONDENSED_QP_H
