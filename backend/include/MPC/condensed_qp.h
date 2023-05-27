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
 * @brief Set the One Matrix object
 * 
 * @param P 
 * @param W 
 * @param n_CV 
 * @return SparseXd 
 */
SparseXd setOneMatrix(int P, int W, int n_CV);

/**
 * @brief Set the weight Matrices @param Q_bar and @param R_bar
 * 
 * @param Q_bar Eigen::SparseMatrix<double> to be filled by output tuning
 * @param R_bar Eigen::SparseMatrix<double> to be filled by change of input tuning
 * @param conf MPCConfig
 */
void setWeightMatrices(SparseXd& Q_bar, SparseXd& R_bar, const MPCConfig& conf);

/**
 * @brief Set the Hessian Matrix G
 * 
 * @param Q_bar Positive definite Eigen::MatrixXd output tuning matrix
 * @param R_bar Positive definite Eigen::MatrixXd change of input tuning matrix
 * @param one 
 * @param theta MatrixXd Theta matrix describing output predictions
 * @param a dim(du)
 * @param n Number of optimalization variables
 * @param n_CV
 */
SparseXd setHessianMatrix(const SparseXd& Q_bar, const SparseXd& R_bar, const SparseXd& one, const MatrixXd& theta, int a, int n, int n_CV); 

/**
 * @brief Set the Gradient Vector q
 * 
 * @param q Eigen::VectorXd gradient vector
 * @param fsr Finite step response model
 * @param Q_bar output tuning
 * @param ref Reference vector 
 * @param conf MPCconfig
 * @param n Number of optimalization variables
 * @param k MPC simulation step, concatinating y_ref
 */
void setGradientVector(VectorXd& q, FSRModel& fsr, const SparseXd& Q_bar,
                        const MatrixXd& ref, const MPCConfig& conf, int n, int k);

/**
 * @brief Set the Constraint Vectors l, u. fsr model is k-dependant
 * 
 * @param l Eigen::VectorXd lower constraint vector 
 * @param u Eigen::VectorXd upper constraint vector
 * @param fsr Finite step response model
 * @param c_l Constant part of lower constraint
 * @param c_u Constant part of upper constraint
 * @param K_inv Inverse of actuation decomposition
 * @param Gamma Gamma vector
 * @param m Number of constraints
 * @param a dim(du)
 */
void setConstraintVectors(VectorXd& l, VectorXd& u, FSRModel& fsr, const VectorXd& c_l, const VectorXd& c_u, const MatrixXd& K_inv,
                         const SparseXd& Gamma, int m, int a);

/**
 * @brief Set the Constraint Matrix A
 * 
 * @param theta FSRM step response predictions
 * @param m Number of constraints
 * @param n Number of optimization variables
 * @param a dim(du)
 * @param n_CV number of controlled variables
 * @return Eigen::Sparse<double>
 */
SparseXd setConstraintMatrix(const MatrixXd& theta, int m, int n, int a, int n_CV);

/**
 * @brief Define constant part of constraints, denoted c_l & c_u
 * 
 * @param z_pop Populated constraints 
 * @param m Number of contraints
 * @param a Number of predicted actuations
 * @param upper bool
 * @return Eigen::VectorXd
 */
VectorXd ConfigureConstraint(const VectorXd& z_pop, int m, int a, bool upper);

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
 * @brief Set the Omega U object, such that du = omega_u * z
 * 
 * @param M Control horizon
 * @param n_MV number of manipulated variables
 * @return Eigen::Sparse<double>
 */
SparseXd setOmegaU(int M, int n_MV);

/**
 * @brief Populate constraint data, enabling dynamic constraints
 * 
 * @param c Constrain vector data
 * @param conf
 * @param a dim(du)
 * @param n_MV Number of manipulated variables
 * @param n_CV Number of constrained variables
 * @return VectorXd, populated vector
 */
VectorXd PopulateConstraints(const VectorXd& c, const MPCConfig& conf, int a, int n_MV, int n_CV);

#endif // CONDENSED_QP_H
