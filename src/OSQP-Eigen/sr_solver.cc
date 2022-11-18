/**
 * @file sr_solver.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */
#include "sr_solver.h"
#include "json_specifiers.h"
#include "data_objects.h"

#include "OsqpEigen/OsqpEigen.h"
#include <Eigen/Eigen>

#include <string>
#include <iostream>
#include <stdexcept>

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

void setKmatrix(SparseXd& K, int M, int n_MV) {
    MatrixXd K_arg = MatrixXd::Zero(M, M);
    std::array<double, 2> arr = {1.0, -1.0};
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < arr.size(); j++) { // NB M > 2. 
            if (i == M-1 && j == 1) {
                break;
            }
            K_arg(i+j, i) = arr[j];
        }
    }
    blkdiag(K, K_arg, n_MV);
}

void blkdiag(SparseXd& blk_mat, const MatrixXd& arg, int count) {
    MatrixXd mat = MatrixXd::Zero(arg.rows() * count, arg.cols() * count);
    for (int i = 0; i < count; i++) {
        mat.block(i * arg.rows(), i * arg.cols(), arg.rows(), arg.cols()) = arg;
    }
    blk_mat = mat.sparseView();
}

void setKInv(MatrixXd& K_inv, int n) { 
    K_inv = MatrixXd::Constant(n, n, 1);
    K_inv = K_inv.triangularView<Eigen::Lower>();
}

void setGamma(SparseXd& gamma, int M, int n_MV) {
    MatrixXd gamma_arg = MatrixXd::Zero(M, 1);
    gamma_arg(0, 0) = 1.0;
    blkdiag(gamma, gamma_arg, n_MV);
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

void setConstraintVectors(VectorXd& l, VectorXd& u, const VectorXd& z_min, const VectorXd& z_max,
                        const FSRModel& fsr, int m, int n) {
    l.resize(z_min.rows());
    u.resize(z_max.rows());
    
    MatrixXd K_inv;
    setKInv(K_inv, n);
    SparseXd gamma; 
    setGamma(gamma, fsr.getM(), fsr.getN_MV());

    VectorXd c(m);
    c.block(0, 0, n, 1) = VectorXd::Zero(n);
    c.block(n, 0, n, 1) = K_inv * gamma * fsr.getUK();
    c.block(2 * n, 0, m - 2 * n , 1) = fsr.getLambda(); // m - 2n = P * n_{CV}

    l = z_min - c;
    u = z_max - c;
}

void setTau(VectorXd& tau, VectorXd* y_ref, int P, int W, int n_CV) {
    tau.resize(n_CV * P);
    for (int i = 0; i < n_CV; i++) {
        tau.block(i * n_CV, 0, P, 1) = y_ref[i](Eigen::seq(0, P-1)); // Bit unsure on how to do time delay
    }
}

void setGradientVector(VectorXd& q, const FSRModel& fsr, const SparseXd& Q_bar,
                        VectorXd* y_ref) {
    VectorXd tau;
    setTau(tau, y_ref, fsr.getP(), fsr.getW(), fsr.getN_CV());
    q = 2 * fsr.getTheta().transpose() * Q_bar * (fsr.getLambda() - tau);
}

void setDelta(SparseXd delta, int M, int n_MV) {
    MatrixXd delta_dense = MatrixXd::Zero(n_MV, n_MV * M);
    for (int i = 0; i < n_MV; i++) {
        delta_dense(i, i * M) = 1;
    }
    delta = delta_dense.sparseView();
}

void sr_solver(int T, FSRModel& fsr, const MPCConfig& conf, const VectorXd& z_min, 
                const VectorXd& z_max, VectorXd* y_ref) {
    OsqpEigen::Solver solver;
    solver.settings()->setWarmStart(true); // Starts primal and dual variables from previous QP
    // MPC Scenario variables
    int M = fsr.getM();
    int P = fsr.getP();
    int n_MV = fsr.getN_MV();
    int n_CV = fsr.getN_CV();

    // Define QP
    const int n = M * n_MV; // Optimization variables 
    const int m = P * n_CV + 2 * n; // Constraints

    solver.data()->setNumberOfVariables(n);
    solver.data()->setNumberOfConstraints(m);

    // Define Cost function variables: 
    SparseXd Q_bar; 
    SparseXd R_bar; 
    SparseXd G;
    SparseXd A;

    VectorXd q;
    VectorXd l;
    VectorXd u; 

    setWeightMatrices(Q_bar, R_bar, conf);
    setHessianMatrix(G, Q_bar, R_bar, fsr);
    setGradientVector(q, fsr, Q_bar, y_ref);
    setConstraintMatrix(A, fsr, m, n);
    setConstraintVectors(l, u, z_min, z_max, fsr, m, n);

    if (!solver.data()->setHessianMatrix(G)) { throw std::runtime_error("Cannot initialize Hessian"); }
    if (!solver.data()->setGradient(q)) { throw std::runtime_error("Cannot initialize Gradient"); }
    if (!solver.data()->setLinearConstraintsMatrix(A)) { throw std::runtime_error("Cannot initialize constraint matrix"); }
    if (!solver.data()->setLowerBound(l)) { throw std::runtime_error("Cannot initialize lower bound"); }
    if (!solver.data()->setUpperBound(u)) { throw std::runtime_error("Cannot initialize upper bound"); }
    if (!solver.initSolver()) { throw std::runtime_error("Cannot initialize solver"); }

    MatrixXd du_mat = MatrixXd::Zero(n_MV, T);
    SparseXd gamma;
    setGamma(gamma, M, n_MV);
    SparseXd delta;
    setDelta(delta, M, n_MV);

    for (int k = 0; k < T; k++) {
        // solve the QP problem
        if (solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) { throw std::runtime_error("Cannot solve problem"); }

        // Claim solution
        VectorXd z = solver.getSolution().transpose(); // row vector
        VectorXd du = delta * z; 

        // Store optimal du
        //du_mat(Eigen::seq(0, n_MV), k) = du(Eigen::seq(0, Eigen::last));

        // Propagate the model
        //fsr.UpdateU(du, gamma * z);

        // update MPC problem
        // updateConstraintVectors(x0, lowerBound, upperBound);

        // Check if bounds are valid:
        // if (!solver.updateBounds(lowerBound, upperBound)) { throw std::runtime_error("Cannot update problem"); }
    }
}