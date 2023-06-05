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
 * @brief Set the One Matrix, slack scaling matric
 * 
 * @param P Prediction horizon
 * @param W Start horizon
 * @param n_CV number of controlled variables
 * @return SparseXd 
 */
SparseXd setOneMatrix(int P, int W, int n_CV);

/**
 * @brief Set the weight Matrices @param Q_bar and @param R_bar
 * 
 * @param Q_bar Output error penalty matrix
 * @param R_bar Actuation penalty matrix
 * @param conf MPCConfig
 */
void setWeightMatrices(SparseXd& Q_bar, SparseXd& R_bar, const MPCConfig& conf);

/**
 * @brief Set the Hessian Matrix G_cd
 * 
 * @param Q_bar Positive definite Eigen::MatrixXd output tuning matrix
 * @param R_bar Positive definite Eigen::MatrixXd change of input tuning matrix
 * @param one scaling matrix
 * @param theta MatrixXd Theta matrix describing output predictions
 * @param a dim(du)
 * @param n Number of optimalization variables
 * @param n_CV number of controlled variables
 */
SparseXd setHessianMatrix(const SparseXd& Q_bar, const SparseXd& R_bar, const SparseXd& one, const MatrixXd& theta, int a, int n, int n_CV); 

/**
 * @brief Set the Gradient Vector @param q
 * 
 * @param q Eigen::VectorXd gradient vector
 * @param fsr Finite step response model
 * @param Q_bar output tuning
 * @param one scaling matrix
 * @param ref Reference vector 
 * @param conf MPCconfig
 * @param n Number of optimalization variables
 * @param k MPC simulation step, concatinating y_ref
 */
void setGradientVector(VectorXd& q, FSRModel& fsr, const SparseXd& Q_bar, const SparseXd& one,
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
 * @param one scaling matrix
 * @param theta FSRM step response predictions
 * @param K_inv Inverse of actuation decomposition
 * @param m Number of constraints
 * @param n Number of optimization variables
 * @param a dim(du)
 * @param n_CV number of controlled variables
 * @return Eigen::Sparse<double>
 */
SparseXd setConstraintMatrix(const SparseXd& one, const MatrixXd& theta, const MatrixXd& K_inv, int m, int n, int a, int n_CV);

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
 * @param conf MPC configuration
 * @param a dim(du)
 * @param n_MV Number of manipulated variables
 * @param n_CV Number of constrained variables
 * @return VectorXd, populated vector
 */
VectorXd PopulateConstraints(const VectorXd& c, const MPCConfig& conf, int a, int n_MV, int n_CV);

/**
 * @brief Set the Hessian Matrix G_cd object for condensed controller without slack
 * 
 * @param Q_bar Output error penalty matrix
 * @param R_bar Actuation penalty matrix
 * @param theta FSRM prediction matrix
 * @return SparseXd 
 */
SparseXd setHessianMatrixWoSlack(const SparseXd& Q_bar, const SparseXd& R_bar, const MatrixXd& theta);

/**
 * @brief Set the Gradient Vector @param q object for condensed controller without slack
 * 
 * @param q Eigen::VectorXd gradient vector
 * @param fsr Finite Step Response model
 * @param Q_bar Output error penalty matrix
 * @param ref Reference vector
 * @param n Number of optimization variables
 * @param k MPC simulation step, concatinating y_ref
 */
void setGradientVectorWoSlack(VectorXd& q, FSRModel& fsr, const SparseXd& Q_bar, const MatrixXd& ref, int n, int k);

/**
 * @brief Set the Constraint Matrix A object for condensed controller without slack
 * 
 * @param theta FSRM prediction matrix
 * @param K_inv Inverse of actuation decomposition
 * @param m Number of constraints
 * @param n Number of optimization variables
 * @param n_CV Number of controlled variables
 * @return SparseXd 
 */
SparseXd setConstraintMatrixWoSlack(const MatrixXd& theta, const MatrixXd& K_inv, int m, int n, int n_CV);

/**
 * @brief Set the Constraint Vectors l, u object for condensed controller without slack
 * 
 * @param l Eigen::VectorXd lower constraint vector 
 * @param u Eigen::VectorXd upper constraint vector
 * @param fsr Finite step response model
 * @param c_l Constant part of lower constraint
 * @param c_u Constant part of upper constraint
 * @param K_inv Inverse of actuation decomposition
 * @param Gamma Gamma vector
 * @param m Number of constraints
 * @param n Number of optimization variables
 */
void setConstraintVectorsWoSlack(VectorXd& l, VectorXd& u, FSRModel& fsr, const VectorXd& c_l, const VectorXd& c_u, const MatrixXd& K_inv,
                         const SparseXd& Gamma, int m, int n);
#endif // CONDENSED_QP_H
