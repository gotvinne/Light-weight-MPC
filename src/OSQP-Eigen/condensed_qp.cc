/**
 * @file condensed_qp.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */
#include "OSQP-Eigen/condensed_qp.h"
#include "IO/data_objects.h"
#include "model/FSRModel.h"

#include <string>
#include <iostream>

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
 * @brief Helper function. Implementing block diagonal matrix
 * 
 * @param blk_mat Eigen::SparseMatrix<double> to be block diagonalized
 * @param vec Diagonal vector
 * @param count Number of blocks
 */
// static void blkdiag(SparseXd& blk_mat, const VectorXd& vec, int count) {
//     MatrixXd arg = vec.asDiagonal();
//     MatrixXd mat = MatrixXd::Zero(arg.rows() * count, arg.cols() * count);
//     for (int i = 0; i < count; i++) {
//         mat.block(i * arg.rows(), i * arg.cols(), arg.rows(), arg.cols()) = arg;
//     }
//     blk_mat = mat.sparseView();
// }

/**
 * @brief Calculate K inv matrix, this is a lower triangular matrix
 * 
 * @param K_inv Eigen::MatrixXd passed by reference
 * @param n size of K_inv
 */
static void setKInv(MatrixXd& K_inv, int n) { 
    K_inv = MatrixXd::Constant(n, n, 1);
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
 * @brief Set the Bounds object
 * 
 * @param bound Eigen::VectorXd representing a bound constraint
 * @param z_pop Eigen::VectorXd constraint vector
 * @param fsr FSRModel Finite step response model
 * @param K_inv Eigen::MatrixXd lower triangular matrix
 * @param Gamma Eigen::SparseXd 
 * @param m Number of constraints 
 * @param n Number of optimalization variables
 */
static void setBounds(VectorXd& bound, const VectorXd& z_pop, FSRModel& fsr, const MatrixXd& K_inv, 
                const SparseXd& Gamma, int m, int n) {
    bound.resize(m);
    VectorXd c(m);

    // c = [ O (n, n),
    //       K⁽⁻¹⁾ Gamma U(k-N),
    //       Lambda]

    c.block(0, 0, n, 1) = VectorXd::Zero(n);
    c.block(n, 0, n, 1) = K_inv * Gamma * fsr.getUK();
    c.block(2 * n, 0, m - 2 * n , 1) = fsr.getLambda(); // m - 2n = P * n_CV

    bound = z_pop - c;
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

void setHessianMatrix(SparseXd& G, const SparseXd& Q_bar, const SparseXd& R_bar, const FSRModel& fsr) {
    int m = fsr.getM() * fsr.getN_MV();
    G.resize(m, m);
    G = 2 * fsr.getTheta().transpose() * Q_bar * fsr.getTheta() + 2 * R_bar;
}

void setGradientVector(VectorXd& q, FSRModel& fsr, const SparseXd& Q_bar,
                        VectorXd* y_ref, int k) {
    VectorXd tau;
    setTau(tau, y_ref, fsr.getP(), fsr.getN_CV(), k);
    q = 2 * fsr.getTheta().transpose() * Q_bar * (fsr.getLambda() - tau);
}

void setConstraintMatrix(SparseXd& A, const FSRModel& fsr, int m, int n) {
    A.resize(m, n);
    MatrixXd dense = MatrixXd::Zero(m, n); 

    // A = [ I_nxn,
    //       K⁽⁻¹⁾,
    //       Theta]; 

    MatrixXd I = MatrixXd::Identity(n, n); 
    MatrixXd K_inv;
    setKInv(K_inv, n);

    dense.block(0, 0, n, n) = I;
    dense.block(n, 0, n, n) = K_inv;
    dense.block(2 * n, 0, fsr.getP() * fsr.getN_CV(), n) = fsr.getTheta();
    A = dense.sparseView();
}

void setConstraintVectors(VectorXd& l, VectorXd& u, const VectorXd& z_min_pop, const VectorXd& z_max_pop,
                         FSRModel& fsr, int m, int n) {
    MatrixXd K_inv;
    setKInv(K_inv, n);
    SparseXd Gamma; 
    setGamma(Gamma, fsr.getM(), fsr.getN_MV());

    setBounds(l, z_min_pop, fsr, K_inv, Gamma, m, n); // Constrain lower bound
    setBounds(u, z_max_pop, fsr, K_inv, Gamma, m, n); // Constrain upper bound
}

void setOmegaU(SparseXd& omega, int M, int n_MV) {
    MatrixXd omega_dense = MatrixXd::Zero(n_MV, n_MV * M);
    for (int i = 0; i < n_MV; i++) {
        omega_dense(i, i * M) = 1;
    }
    omega = omega_dense.sparseView();
}

VectorXd PopulateConstraints(const VectorXd& c, int n_MV, int n_CV, int M, int P) { 
    int n = M * n_CV + 2 * n_CV;
    VectorXd populated = VectorXd::Zero(P * n_CV + 2 * n); // populated.rows() = m

    // z_pop = [ z - Delta U (M * N_MV),
    //           z - U (M * N_MV),
    //           z - Y (P * N_CV),
    //           0 (N_CV),
    //           0 (N_CV)]

    for (int var = 0; var < 2 * n_MV; var++) { // Assuming same constraining, u, du if n_MV < n_CV
        populated.block(var * M, 0, M, 1) = VectorXd::Constant(M, c(var));
    } // Fill n first constraints, du and u, 2 * M * N_MV

    for (int var = 0; var < n_CV; var++) {
        populated.block(2 * n + (var * P), 0, P, 1) = VectorXd::Constant(P, c(2 * n_MV + var));
    } // Fill remaining constraints, y, P * N_CV
    return populated;
} 
