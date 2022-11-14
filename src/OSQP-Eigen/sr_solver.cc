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

void setWeightMatrices(Eigen::SparseMatrix<float>& Q_bar, Eigen::SparseMatrix<float>& R_bar, 
                        const MPCConfig& mpc_config) { // Consider making Q_bar, R_bar SparseMatrix
    Eigen::MatrixXf Q = Eigen::MatrixXf::Zero(mpc_config.P - mpc_config.W, mpc_config.P - mpc_config.W);
    Eigen::MatrixXf R = Eigen::MatrixXf::Zero(mpc_config.M, mpc_config.M);

    Q = mpc_config.Q.asDiagonal();
    R = mpc_config.R.asDiagonal();

    Q_bar = Q.sparseView();
    R_bar = R.sparseView();
}

void setHessianMatrix(Eigen::SparseMatrix<float>& G, const Eigen::MatrixXf& theta, const Eigen::SparseMatrix<float>& Q_bar, 
                        const Eigen::SparseMatrix<float>& R_bar, int n_MV, int M) {
    G.resize(M*n_MV, M*n_MV);
    G = 2*theta.transpose()*Q_bar*theta + 2*R_bar;
}

void setKmatrix(Eigen::SparseMatrix<float>& K, int M, int n_MV) {
    Eigen::MatrixXf K_arg = Eigen::MatrixXf::Zero(M, M);
    std::array<float, 2> arr = {1.0, -1.0};
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

void blkdiag(Eigen::SparseMatrix<float>& blk_mat, const Eigen::MatrixXf& arg, int count) {
    Eigen::MatrixXf mat = Eigen::MatrixXf::Zero(arg.rows() * count, arg.cols() * count);
    for (int i = 0; i < count; i++) {
        mat.block(i * arg.rows(), i * arg.cols(), arg.rows(), arg.cols()) = arg;
    }
    blk_mat = mat.sparseView();
}

void setKInv(Eigen::MatrixXf& K_inv, int M) {
    K_inv = Eigen::MatrixXf::Constant(M, M, 1);
    K_inv = K_inv.triangularView<Eigen::Lower>();
}

void setGamma(Eigen::SparseMatrix<float>& gamma, int M, int n_MV) {
    Eigen::MatrixXf gamma_arg = Eigen::MatrixXf::Zero(M, 1);
    gamma_arg(0, 0) = 1.0;
    blkdiag(gamma, gamma_arg, n_MV);
}

void setConstraintMatrix(Eigen::SparseMatrix<float>& A, const FSRModel& fsr, const int& m, const int& n) {
    A.resize(m, n);
    Eigen::MatrixXf dense = Eigen::MatrixXf::Zero(m, n); 
    Eigen::MatrixXf K_inv;
    setKInv(K_inv, fsr.getM());

    dense.block(n, 0, n, n) = K_inv;
    dense.block(2 * n, 0, fsr.getP() * fsr.getN_CV(), n) = fsr.getTheta();
    A = dense.sparseView();
}

void setConstrainVectors(Eigen::VectorXf& l, Eigen::VectorXf& u, const Eigen::VectorXf& z_max, const Eigen::VectorXf& z_min,
                        const Eigen::VectorXf& lambda, const Eigen::VectorXf& u_k, int M, int n_MV, int n_CV) {
    l.resize(z_min.rows());
    u.resize(z_max.rows());
    
    Eigen::MatrixXf K_inv;
    setKInv(K_inv, M);

    Eigen::SparseMatrix<float> gamma; 
    setGamma(gamma, M, n_MV);

    Eigen::VectorXf c(2 * n_MV + n_CV);
    c.block(0, 0, n_MV, 1) = Eigen::VectorXf::Zero(n_MV);
    c.block(n_MV, 0, n_MV, 1) = K_inv * gamma * u_k; // You were here!
    c.block(2 * n_MV, 0, 2 * n_MV + n_CV, 1) = lambda;

    l = z_min - c;
    u = z_max - c;
}

void sr_solver(const int& T, const FSRModel& fsr, const MPCConfig& conf, const Eigen::VectorXf& z_min, 
                const Eigen::VectorXf& z_max) { // Might consider only feeding R and Q

    OsqpEigen::Solver solver;
    solver.settings()->setWarmStart(true); // Starts primal and dual variables from previous QP

    // Define QP
    const int n = fsr.getM() * fsr.getN_MV(); // Optimization variables 
    const int m = fsr.getP() * fsr.getN_CV() + 2 * n; // Constraints

    solver.data()->setNumberOfVariables(n);
    solver.data()->setNumberOfConstraints(m);

    Eigen::SparseMatrix<float> Q_bar; 
    Eigen::SparseMatrix<float> R_bar; 
    Eigen::SparseMatrix<float> G;
    Eigen::SparseMatrix<float> A;
    Eigen::SparseMatrix<float> q;

    Eigen::VectorXf l;
    Eigen::VectorXf u; 

    setWeightMatrices(Q_bar, R_bar, conf);
    setHessianMatrix(G, fsr.getTheta(), Q_bar, R_bar, fsr.getN_MV(), fsr.getM());
    setConstraintMatrix(A, fsr, m, n);


    setConstrainVectors(l, u, z_min, z_max, fsr.getLambda(), fsr.getUK(), fsr.getM(), fsr.getN_MV(), fsr.getN_CV());

    if (!solver.data()->setHessianMatrix(G)) { throw std::runtime_error("Cannot initialize Hessian"); }
    // if (!solver.data()->setGradient(q)) { throw std::runtime_error("Cannot initialize Gradient"); }
    if(!solver.data()->setLinearConstraintsMatrix(A)) { throw std::runtime_error("Cannot initialize constraint matrix"); }
    // if (!solver.data()->setLowerBound(l)) { throw std::runtime_error("Cannot initialize lower bound"); }
    // if (!solver.data()->setUpperBound(u)) { throw std::runtime_error("Cannot initialize upper bound"); }

    // if (!solver.initSolver()) { // If solver cannot be initialized
    //     throw std::runtime_error("Cannot initialize solver");
    // }

    // controller input and QPSolution vector
    Eigen::MatrixXf du = Eigen::MatrixXf::Zero(fsr.getN_MV(), T);

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