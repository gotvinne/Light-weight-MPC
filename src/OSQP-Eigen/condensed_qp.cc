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
 * @brief Calculate K matrix
 * 
 * @param K Eigen::SparseMatrix<double> to be filled 
 * @param M Control horizon
 * @param n_MV number of manupulated variables
 */
// static void setKmatrix(SparseXd& K, int M, int n_MV) {
//     MatrixXd K_arg = MatrixXd::Zero(M, M);
//     std::array<double, 2> arr = {1.0, -1.0};
//     for (int i = 0; i < M; i++) {
//         for (int j = 0; j < int(arr.size()); j++) { // NB M > 2. 
//             if (i == M-1 && j == 1) {
//                 break;
//             }
//             K_arg(i+j, i) = arr[j];
//         }
//     }
//     blkdiag(K, K_arg, n_MV);
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
 * @brief Set the Gamma object
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
        tau.block(i * n_CV, 0, P, 1) = y_ref[i](Eigen::seq(k, k + (P-1))); // Bit unsure on how to do time delay
    }
}

void setWeightMatrices(SparseXd& Q_bar, SparseXd& R_bar, const MPCConfig& mpc_config) {
    MatrixXd Q = MatrixXd::Zero(mpc_config.P - mpc_config.W, mpc_config.P - mpc_config.W);
    MatrixXd R = MatrixXd::Zero(mpc_config.M, mpc_config.M);
    Q = mpc_config.Q.asDiagonal();
    R = mpc_config.R.asDiagonal();

    Q_bar = Q.sparseView();
    R_bar = R.sparseView();
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
    MatrixXd K_inv;
    setKInv(K_inv, n);

    dense.block(n, 0, n, n) = K_inv;
    dense.block(2 * n, 0, fsr.getP() * fsr.getN_CV(), n) = fsr.getTheta();
    A = dense.sparseView();
}

void setConstraintVectors(VectorXd& l, VectorXd& u, const VectorXd& z_min_pop, const VectorXd& z_max_pop,
                         FSRModel& fsr, int m, int n) {
    l.resize(m);
    u.resize(m);
    
    MatrixXd K_inv;
    setKInv(K_inv, n);
    SparseXd gamma; 
    setGamma(gamma, fsr.getM(), fsr.getN_MV());

    VectorXd c(m);
    c.block(0, 0, n, 1) = VectorXd::Zero(n);
    c.block(n, 0, n, 1) = K_inv * gamma * fsr.getUK();
    c.block(2 * n, 0, m - 2 * n , 1) = fsr.getLambda(); // m - 2n = P * n_CV

    l = z_min_pop - c;
    u = z_max_pop - c;
}

void setOmegaU(SparseXd& omega, int M, int n_MV) {
    MatrixXd omega_dense = MatrixXd::Zero(n_MV, n_MV * M);
    for (int i = 0; i < n_MV; i++) {
        omega_dense(i, i * M) = 1;
    }
    omega = omega_dense.sparseView();
}

VectorXd PopulateConstraints(const VectorXd& c, int m, int n) {
    VectorXd populated(m);
    // Accessing the constraints for du, u and y, given in respectively 1st, 2nd and 3rd position
    populated.block(0, 0, n, 1) = VectorXd::Constant(n, c(0));
    populated.block(n, 0, n, 1) = VectorXd::Constant(n, c(1));
    populated.block(2 * n, 0, m - 2 * n, 1) = VectorXd::Constant(m - 2 * n, c(2)); // m - 2n = P * n_CV
    return populated;
} 
