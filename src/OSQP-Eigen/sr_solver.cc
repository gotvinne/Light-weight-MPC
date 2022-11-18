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

void setWeightMatrices(Eigen::SparseMatrix<double>& Q_bar, Eigen::SparseMatrix<double>& R_bar, 
                        const MPCConfig& mpc_config) { // Consider making Q_bar, R_bar SparseMatrix
    Eigen::MatrixXd Q = Eigen::MatrixXd::Zero(mpc_config.P - mpc_config.W, mpc_config.P - mpc_config.W);
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(mpc_config.M, mpc_config.M);
    Q = mpc_config.Q.asDiagonal();
    R = mpc_config.R.asDiagonal();

    Q_bar = Q.sparseView();
    R_bar = R.sparseView();
}

void setHessianMatrix(Eigen::SparseMatrix<double>& G, const Eigen::MatrixXd& theta, const Eigen::SparseMatrix<double>& Q_bar, 
                        const Eigen::SparseMatrix<double>& R_bar, int n_MV, int M) {
    G.resize(M*n_MV, M*n_MV);
    G = 2*theta.transpose()*Q_bar*theta + 2*R_bar;
}

void setKmatrix(Eigen::SparseMatrix<double>& K, int M, int n_MV) {
    Eigen::MatrixXd K_arg = Eigen::MatrixXd::Zero(M, M);
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

void blkdiag(Eigen::SparseMatrix<double>& blk_mat, const Eigen::MatrixXd& arg, int count) {
    Eigen::MatrixXd mat = Eigen::MatrixXd::Zero(arg.rows() * count, arg.cols() * count);
    for (int i = 0; i < count; i++) {
        mat.block(i * arg.rows(), i * arg.cols(), arg.rows(), arg.cols()) = arg;
    }
    blk_mat = mat.sparseView();
}

void setKInv(Eigen::MatrixXd& K_inv, int M) {
    K_inv = Eigen::MatrixXd::Constant(M, M, 1);
    K_inv = K_inv.triangularView<Eigen::Lower>();
}

void setGamma(Eigen::SparseMatrix<double>& gamma, int M, int n_MV) {
    Eigen::MatrixXd gamma_arg = Eigen::MatrixXd::Zero(M, 1);
    gamma_arg(0, 0) = 1.0;
    blkdiag(gamma, gamma_arg, n_MV);
}

void setConstraintMatrix(Eigen::SparseMatrix<double>& A, const FSRModel& fsr, const int& m, const int& n) {
    A.resize(m, n);
    Eigen::MatrixXd dense = Eigen::MatrixXd::Zero(m, n); 
    Eigen::MatrixXd K_inv;
    setKInv(K_inv, fsr.getM());

    dense.block(n, 0, n, n) = K_inv;
    dense.block(2 * n, 0, fsr.getP() * fsr.getN_CV(), n) = fsr.getTheta();
    A = dense.sparseView();
}

void setConstrainVectors(Eigen::VectorXd& l, Eigen::VectorXd& u, const Eigen::VectorXd& z_min, const Eigen::VectorXd& z_max,
                        const Eigen::VectorXd& lambda, const Eigen::VectorXd& u_k, const int& M, const int& n_MV, const int& m) {
    int n = M * n_MV; 
    l.resize(z_min.rows());
    u.resize(z_max.rows());
    
    Eigen::MatrixXd K_inv;
    setKInv(K_inv, M);
    Eigen::SparseMatrix<double> gamma; 
    setGamma(gamma, M, n_MV);

    Eigen::VectorXd c(m);
    c.block(0, 0, n, 1) = Eigen::VectorXd::Zero(n);
    c.block(n, 0, n, 1) = K_inv * gamma * u_k;
    c.block(2 * n, 0, m - 2 * n , 1) = lambda; // m - 2n = P * n_{CV}

    l = z_min - c;
    u = z_max - c;
}

void setTau(Eigen::VectorXd& tau, Eigen::VectorXd* y_ref, const int& P, const int& W, const int& n_CV) {
    tau.resize(n_CV * P);
    for (int i = 0; i < n_CV; i++) {
        tau.block(i * n_CV, 0, P, 1) = y_ref[i](Eigen::seq(0, P-1)); // Bit unsure on how to do time delay
    }
}

void setGradientVector(Eigen::VectorXd& q, const Eigen::VectorXd& lambda, const Eigen::MatrixXd& theta, const Eigen::SparseMatrix<double>& Q_bar,
                        Eigen::VectorXd* y_ref, const int& P, const int& W, const int& n_CV) {
    Eigen::VectorXd tau;
    setTau(tau, y_ref, P, W, n_CV);
    q = 2 * theta.transpose() * Q_bar * (lambda - tau);
}

void sr_solver(const int& T, const FSRModel& fsr, const MPCConfig& conf, const Eigen::VectorXd& z_min, 
                const Eigen::VectorXd& z_max, Eigen::VectorXd* y_ref) { // Might consider only feeding R and Q

    OsqpEigen::Solver solver;
    solver.settings()->setWarmStart(true); // Starts primal and dual variables from previous QP

    // Define QP
    const int n = fsr.getM() * fsr.getN_MV(); // Optimization variables 
    const int m = fsr.getP() * fsr.getN_CV() + 2 * n; // Constraints

    solver.data()->setNumberOfVariables(n);
    solver.data()->setNumberOfConstraints(m);

    Eigen::SparseMatrix<double> Q_bar; 
    Eigen::SparseMatrix<double> R_bar; 
    Eigen::SparseMatrix<double> G;
    Eigen::SparseMatrix<double> A;

    Eigen::VectorXd q;
    Eigen::VectorXd l;
    Eigen::VectorXd u; 

    setWeightMatrices(Q_bar, R_bar, conf);
    setHessianMatrix(G, fsr.getTheta(), Q_bar, R_bar, fsr.getN_MV(), fsr.getM());
    setGradientVector(q, fsr.getLambda(), fsr.getTheta(), Q_bar, y_ref, fsr.getP(), fsr.getW(), fsr.getN_CV());
    setConstraintMatrix(A, fsr, m, n);
    setConstrainVectors(l, u, z_min, z_max, fsr.getLambda(), fsr.getUK(), fsr.getM(), fsr.getN_MV(), m);

    if (!solver.data()->setHessianMatrix(G)) { throw std::runtime_error("Cannot initialize Hessian"); }
    if (!solver.data()->setGradient(q)) { throw std::runtime_error("Cannot initialize Gradient"); }
    if (!solver.data()->setLinearConstraintsMatrix(A)) { throw std::runtime_error("Cannot initialize constraint matrix"); }
    if (!solver.data()->setLowerBound(l)) { throw std::runtime_error("Cannot initialize lower bound"); }
    if (!solver.data()->setUpperBound(u)) { throw std::runtime_error("Cannot initialize upper bound"); }

    if (!solver.initSolver()) { throw std::runtime_error("Cannot initialize solver"); }

    // controller input and QPSolution vector
    Eigen::MatrixXd du = Eigen::MatrixXd::Zero(fsr.getN_MV(), T);

    // for (int i = 0; i < T; i++) {

    //     // solve the QP problem
    //     if(solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) { throw std::runtime_error("Cannot solve problem"); }

    //     // Claim solution
    //     du = solver.getSolution();

    //     // save data into file
    //     auto x0Data = x0.data();

    //     // propagate the model
    //     x0 = a * x0 + b * ctr;

    //     // update the constraint bound
    //     updateConstraintVectors(x0, lowerBound, upperBound);
    //     if (!solver.updateBounds(lowerBound, upperBound)) { throw std::runtime_error("Cannot update problem"); }
    // }

    // Save data to file
}