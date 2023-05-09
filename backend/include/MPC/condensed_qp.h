/**
 * @file condensed_qp.
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */
#ifndef CONDENSED_QP_H
#define CONDENSED_QP_H

#include "IO/data_objects.h"
#include "model/FSRModel.h"

#include <Eigen/Eigen>
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using SparseXd = Eigen::SparseMatrix<double>;

/**
 * @brief Set the weight Matrices
 * 
 * @param Q_bar Eigen::SparseMatrix<double> to be filled by output tuning
 * @param R_bar Eigen::SparseMatrix<double> to be filled by change of input tuning
 * @param conf MPCConfig
 */
void setWeightMatrices(SparseXd& Q_bar, SparseXd& R_bar, const MPCConfig& conf);

/**
 * @brief Set the Hessian Matrix G
 * 
 * @param G Positive definite optimization matrix
 * @param Q_bar Positive definite Eigen::MatrixXd output tuning matrix
 * @param R_bar Positive definite Eigen::MatrixXd change of input tuning matrix
 * @param fsr FSRModel Finite step response model
 * @param a
 * @param n
 */
void setHessianMatrix(SparseXd& G, const SparseXd& Q_bar, const SparseXd& R_bar, const FSRModel& fsr, int a, int n); 

/**
 * @brief Set the Gradient Vector q
 * 
 * @param q Eigen::VectorXd gradient vector
 * @param fsr Finite step response model
 * @param Q_bar output tuning
 * @param y_ref Reference vector 
 * @param conf
 * @param n
 * @param k MPC simulation step, concatinating y_ref
 */
void setGradientVector(VectorXd& q, FSRModel& fsr, const SparseXd& Q_bar,
                        VectorXd* y_ref, const MPCConfig& conf, int n, int k);

/**
 * @brief Set the Constraint Matrix A
 * 
 * @param A Eigen::SparseMatrix<double>
 * @param fsr Finite step response model
 * @param m Number of constraints
 * @param n Number of optimization variables
 * @param a
 */
void setConstraintMatrix(SparseXd& A, const FSRModel& fsr, int m, int n, int a);

/**
 * @brief Set the Constraint Vectors l, u. fsr model is k-dependant
 * 
 * @param l Eigen::VectorXd lower constraint vector 
 * @param u Eigen::VectorXd upper constraint vector
 * @param z_min lower constraints, du, u, y
 * @param z_max upper constraints, du, u, y
 * @param fsr Finite step response model
 * @param m Number of constraints
 * @param n Number of optimization variables
 */
void setConstraintVectors(VectorXd& l, VectorXd& u, const VectorXd& z_min_pop, const VectorXd& z_max_pop,
                         FSRModel& fsr, int m, int n);

/**
 * @brief Set the Omega U object, such that du = omega_u * z
 * 
 * @param Omega_u Sparse Matrix decomposing z
 * @param M Control horizon
 * @param n_MV number of manipulated variables
 */
void setOmegaU(SparseXd& omega, int M, int n_MV);

/**
 * @brief Populate constraint data, enabling dynamic constraints
 * 
 * @param c Constrain vector data
 * @param a
 * @param n_MV Number of manipulated variables
 * @param n_CV Number of constrained variables
 * @param M Control horizon
 * @param P Prediction horizon
 * @return VectorXd, populated vector
 */
VectorXd PopulateConstraints(const VectorXd& c, int a, int n_MV, int n_CV, int M, int P);

/**
 * @brief Calculate K inv matrix, this is a lower triangular matrix
 * 
 * @param a size of K_inv
 * @return Eigen::MatrixXd
 */
MatrixXd setKInv(int a);

/**
 * @brief Set the Gamma object, to select the prior actuation. du_k = gamma * z
 * 
 * @param M Control horizon
 * @param n_MV number of manipulated variables
 * @return Eigen::Sparse<double>
 */
SparseXd setGamma(int M, int n_MV);

/**
 * @brief 
 * 
 * @param bound 
 * @param z_pop Populated constraints 
 * @param m Number of contraints
 * @param a Number of predicted actuations
 * @param upper bool
 */
void ConfigureConstraint(VectorXd& bound, const VectorXd& z_pop, int m, int a, bool upper);

#endif // CONDENSED_QP_H
