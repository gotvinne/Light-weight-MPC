/**
 * @file condensed_qp.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */
#include "OsqpEigen/condensed_qp.h"
#include "IO/data_objects.h"
#include "model/FSRModel.h"

#include <string>
#include <iostream>
#include <limits>

#include <Eigen/Eigen>
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using SparseXd = Eigen::SparseMatrix<double>;

/**
 * @brief Helper function. Implementing block diagonal matrix
 * 
 * @param blk_mat Eigen::SparseMatrix<double> to be block diagonalized
 * @param arg block argument
 * @param count Number of blocks
 */
static void blkdiag(SparseXd& blk_mat, const MatrixXd& arg, int count) {
    MatrixXd mat = MatrixXd::Zero(arg.rows() * count, arg.cols() * count);
    for (int i = 0; i < count; i++) {
        mat.block(i * arg.rows(), i * arg.cols(), arg.rows(), arg.cols()) = arg;
    }
    blk_mat = mat.sparseView();
}

/**
 * @brief Calculate K inv matrix, this is a lower triangular matrix
 * 
 * @param K_inv Eigen::MatrixXd passed by reference
 * @param a size of K_inv
 */
static void setKInv(MatrixXd& K_inv, int a) { 
    K_inv = MatrixXd::Constant(a, a, 1);
    K_inv = K_inv.triangularView<Eigen::Lower>();
}

/**
 * @brief Set the Gamma object, to select the prior actuation.
 * 
 * @param gamma SparseMatrix accessing the first du for every MV, du_k = gamma * z
 * @param M Control horizon
 * @param n_MV number of manipulated variables
 */
static void setGamma(SparseXd& gamma, int M, int n_MV) {
    MatrixXd gamma_arg = MatrixXd::Zero(M, 1);
    gamma_arg(0, 0) = 1.0;
    blkdiag(gamma, gamma_arg, n_MV);
}

/**
 * @brief Set the Tau object, sliced from @param y_ref
 * 
 * @param tau Reference vector on output
 * @param y_ref Reference data
 * @param P Prediction horizon
 * @param n_CV Number of controlled variables
 * @param k MPC simulation step
 */
static void setTau(VectorXd& tau, VectorXd* y_ref, int P, int n_CV, int k) { // Might need W
    tau.resize(n_CV * P);
    for (int i = 0; i < n_CV; i++) {
        tau.block(i * P, 0, P, 1) = y_ref[i](Eigen::seq(k, k + (P-1))); // Bit unsure on how to do time delay
    }
}

/**
 * @brief Extract the k-dependant constraint from constraint vector
 * 
 * @param bound Eigen::VectorXd representing a bound constraint
 * @param fsr FSRModel Finite step response model
 * @param K_inv Eigen::MatrixXd lower triangular matrix
 * @param Gamma Eigen::SparseXd 
 * @param m Number of constraints 
 * @param n Number of optimalization variables
 */
static void UpdateBounds(VectorXd& bound, FSRModel& fsr, const MatrixXd& K_inv, 
                const SparseXd& Gamma, int m, int a) { //Change name
    VectorXd c = VectorXd::Zero(m);
    // c = [ 0 (a),
    //       K⁽⁻¹⁾ Gamma U(k-N) (a),
    //       Lambda (n_CV (P-W)),
    //       Lambda (n_CV (P-W)),
    //       0 (n_CV),
    //       0 (n_CV)]
    int size_lambda = fsr.getLambda().rows();

    c.block(a, 0, a, 1) = K_inv * Gamma * fsr.getUK();
    c.block(2 * a, 0, size_lambda, 1) = fsr.getLambda();
    c.block(2 * a + size_lambda, 0, size_lambda, 1) = fsr.getLambda();

    bound -= c;
}

static void ConfigureConstraint(VectorXd& bound, const VectorXd& z_pop, int m, int a, bool upper) {
    bound = VectorXd::Zero(m);
    int uy_size = z_pop.rows(); // = 2 * M * n_MV + P * n_CV = 2 * a + P * n_CV

    if (upper) {
        bound.block(0, 0, uy_size, 1) = z_pop;
        bound.block(uy_size, 0, m - uy_size, 1) = VectorXd::Constant(m - uy_size, std::numeric_limits<double>::max()); // Infinity
    } else {
        VectorXd tmp = z_pop(Eigen::seq(a, Eigen::indexing::last)); // tmp.rows() = P * n_CV
        bound.block(a, 0, tmp.rows(), 1) = VectorXd::Constant(tmp.rows(), std::numeric_limits<double>::min()); // -Infinity
        bound.block(uy_size, 0, tmp.rows(), 1) = tmp;
    }
}

void setConstraintVectors(VectorXd& l, VectorXd& u, const VectorXd& z_min_pop, const VectorXd& z_max_pop,
                         FSRModel& fsr, int m, int a) {
    MatrixXd K_inv;
    setKInv(K_inv, a);
    SparseXd Gamma; 
    setGamma(Gamma, fsr.getM(), fsr.getN_MV());

    ConfigureConstraint(l, z_min_pop, m, a, false);
    ConfigureConstraint(u, z_max_pop, m, a, true);

    UpdateBounds(l, fsr, K_inv, Gamma, m, a); // Constrain lower bound
    UpdateBounds(u, fsr, K_inv, Gamma, m, a); // Constrain upper bound
}

void setWeightMatrices(SparseXd& Q_bar, SparseXd& R_bar, const MPCConfig& conf) {
    // Replicate and flatten Q and R matrices: 
    MatrixXd Q_replicate = conf.Q.replicate(1, conf.P - conf.W);
    VectorXd Q_flatten = Q_replicate.reshaped<Eigen::RowMajor>().transpose();
    MatrixXd R_replicate = conf.R.replicate(1, conf.M);
    VectorXd R_flatten = R_replicate.reshaped<Eigen::RowMajor>().transpose();

    MatrixXd Q = Q_flatten.asDiagonal();
    MatrixXd R = R_flatten.asDiagonal();

    Q_bar = Q.sparseView(); // dim(Q_bar) = n_CV * (P-W+1) x n_CV * (P-W+1)
    R_bar = R.sparseView(); // dim(R_bar) = n_MV * M x n_MV * M
}

void setHessianMatrix(SparseXd& H, const SparseXd& Q_bar, const SparseXd& R_bar, const FSRModel& fsr, int a, int n) {
    MatrixXd h = MatrixXd::Zero(n, n);
    h.block(0, 0, a, a) = 2 * fsr.getTheta().transpose() * Q_bar * fsr.getTheta() + 2 * R_bar;
    
    // H = [h (axa),
    //      0 (n_CVxn_CV),
    //      0 (n_CVxn_CV)]

    H = h.sparseView();
}

void setGradientVector(VectorXd& q, FSRModel& fsr, const SparseXd& Q_bar,
                        VectorXd* y_ref, const MPCConfig& conf, int n, int k) {
    VectorXd tau;
    setTau(tau, y_ref, fsr.getP(), fsr.getN_CV(), k);

    q.resize(n);
    VectorXd temp = 2 * fsr.getTheta().transpose() * Q_bar * (fsr.getLambda() - tau);
    
    // q = [2 Theta^T Q_bar (Lamba - tau),
    //      rho_{h},
    //      rho_{l}]

    q << temp, conf.RoH, conf.RoL;
}

void setConstraintMatrix(SparseXd& A, const FSRModel& fsr, int m, int n, int a) {
    A.resize(m, n);
    MatrixXd dense = MatrixXd::Zero(m, n); 

    // A = [ I (axa),              0 (axn_CV),         0 (axn_CV)
    //       K⁽⁻¹⁾ (axa),          0 (axn_CV),         0 (axn_CV)
    //       Theta (n_CV(P-W)xa), -I (n_CV(P-W)xn_CV), 0 (n_CV(P-W)xn_CV)
    //       Theta (n_CV(P-W)xa),  0 (n_CV(P-W)xn_CV), I (n_CV(P-W)xn_CV)
    //       0 (n_CVxa),           I (n_CVxn_CV),      0 (n_CVxn_CV)
    //       0 (n_CVxa),           0 (n_CVxn_CV),      I (n_CVxn_CV)]; 

    int n_CV = fsr.getN_CV();
    int dim_theta = fsr.getTheta().rows();

    MatrixXd In_cv = MatrixXd::Identity(n_CV, n_CV);
    MatrixXd Itheta = MatrixXd::Identity(dim_theta, n_CV);
    MatrixXd K_inv;
    setKInv(K_inv, a);

    // dU, U row
    dense.block(0, 0, a, a) = MatrixXd::Identity(a, a);
    dense.block(a, 0, a, a) = K_inv;

    // Y row
    dense.block(2 * a, 0, dim_theta, a) = fsr.getTheta();
    dense.block(2 * a + dim_theta, 0, dim_theta, a) = fsr.getTheta();
    dense.block(2 * a, a, dim_theta, n_CV) = -Itheta;
    dense.block(2 * a + dim_theta, a + n_CV, dim_theta, n_CV) = Itheta;

    // eta row
    dense.block(2 * a + 2 * dim_theta, a, n_CV, n_CV) = In_cv;
    dense.block(2 * a + 2 * dim_theta + n_CV, a + n_CV, n_CV, n_CV) = In_cv;
    A = dense.sparseView();
}

void setOmegaU(SparseXd& omega, int M, int n_MV) {
    MatrixXd omega_dense = MatrixXd::Zero(n_MV, n_MV * M);
    for (int i = 0; i < n_MV; i++) {
        omega_dense(i, i * M) = 1;
    }
    omega = omega_dense.sparseView();
}

VectorXd PopulateConstraints(const VectorXd& c, int a, int n_MV, int n_CV, int M, int P) { 
    VectorXd z_pop(2 * M * n_MV + P * n_CV);

    // z_pop = [ z - Delta U (M * N_MV), 
    //           z - U (M * N_MV),
    //           z - Y (P * N_CV)]

    for (int var = 0; var < 2 * n_MV; var++) { // Assuming same constraining, u, du if n_MV < n_CV
        z_pop.block(var * M, 0, M, 1) = VectorXd::Constant(M, c(var));
    } // Fill n first constraints, du and u, 2 * M * N_MV

    for (int var = 0; var < n_CV; var++) {
        z_pop.block(2 * a + (var * P), 0, P, 1) = VectorXd::Constant(P, c(2 * n_MV + var));
    } // Fill remaining constraints, y, P * N_CV
    return z_pop;
} 
