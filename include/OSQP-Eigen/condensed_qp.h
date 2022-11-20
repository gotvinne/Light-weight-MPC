/**
 * @file condensed_qp.
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
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
 * @brief Set the Constraint Matrix object
 * 
 * @param A Eigen::SparseMatrix<double>
 * @param fsr Finite step response model
 * @param m Number of constraints
 * @param n Number of optimization variables
 */
void setConstraintMatrix(SparseXd& A, const FSRModel& fsr, int m, int n);

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
 * @param m Number of constraints 
 * @param n Number of optimalization variables
 * @return VectorXd, populated vector
 */
VectorXd PopulateConstraints(const VectorXd& c, int m, int n);

#endif // CONDENSED_QP_H
