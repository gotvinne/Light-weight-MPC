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
#include <Eigen/Dense>

#include <map>
#include <array>
#include <string>
#include <iostream>
#include <stdexcept>

void setWeightMatrices(Eigen::MatrixXf& Q_bar, Eigen::MatrixXf& R_bar, 
                        const MPCConfig& mpc_config) {
    Q_bar.resize(mpc_config.P - mpc_config.W, mpc_config.P - mpc_config.W);
    R_bar.resize(mpc_config.M, mpc_config.M);

    Q_bar = mpc_config.Q.asDiagonal();
    R_bar = mpc_config.R.asDiagonal();
}

void setHessianMatrix(Eigen::MatrixXf& G, const Eigen::MatrixXf& theta, const Eigen::MatrixXf& Q_bar, 
                        const Eigen::MatrixXf& R_bar, int n_MV, int M) {
    G.resize(M*n_MV, M*n_MV);
    G = 2*theta.transpose()*Q_bar*theta + 2*R_bar;
}

void setKmatrix(Eigen::MatrixXf& blk_mat, int M, int n_MV) {
    Eigen::MatrixXf K = Eigen::MatrixXf::Zero(M, M);
    std::array<float, 2> arr = {1.0, -1.0};
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < arr.size(); j++) { // NB M > 2. 
            if (i == M-1 && j == 1) {
                break;
            }
            K(i+j, i) = arr[j];
        }
    }
    blkdiag(blk_mat, K, n_MV);
}

void blkdiag(Eigen::MatrixXf& blk_mat, const Eigen::MatrixXf& arg, int count) {
    blk_mat = Eigen::MatrixXf::Zero(arg.rows() * count, arg.cols() * count);
    for (int i = 0; i < count; i++) {
        blk_mat.block(i * arg.rows(), i * arg.cols(), arg.rows(), arg.cols()) = arg;
    }
}

void setKInv(Eigen::MatrixXf& K_inv, int M) {
    K_inv = Eigen::MatrixXf::Constant(M, M, 1);
    K_inv = K_inv.triangularView<Eigen::Lower>();
}

void setConstraintMatrix(Eigen::MatrixXf& A, const FSRModel& fsr, const int& m, const int& n) {
    A.resize(m, n);
    Eigen::MatrixXf K_inv; 
    setKInv(K_inv, fsr.getM());

    A.block(0, 0, n, n) = Eigen::MatrixXf::Zero(n, n);
    A.block(n, 0, n, n) = K_inv;
    A.block(2 * n, 0, fsr.getP() * fsr.getN_CV(), n) = fsr.getTheta();
}

void setConstrainVectors(Eigen::VectorXf& l, Eigen::VectorXf& u, const Eigen::VectorXf& z_max, const Eigen::VectorXf& z_min) {

}

void sr_solver(const int& T, const FSRModel& fsr, const MPCConfig& conf) { // Might consider only feeding R and Q

    OsqpEigen::Solver solver;
    solver.settings()->setWarmStart(true); // Starts primal and dual variables from previous QP

    // Define QP
    int n = fsr.getM() * fsr.getN_MV(); // Optimization variables 
    int m = fsr.getP() * fsr.getN_CV() + 2 * n; // Constraints

    solver.data()->setNumberOfVariables(n);
    solver.data()->setNumberOfConstraints(m);

    Eigen::MatrixXf Q_bar; 
    Eigen::MatrixXf R_bar; 
    Eigen::MatrixXf G;
    Eigen::MatrixXf A;

    setWeightMatrices(Q_bar, R_bar, conf);
    setHessianMatrix(G, fsr.getTheta(), Q_bar, R_bar, fsr.getM(), fsr.getM());
    setConstraintMatrix(A, fsr, m, n);

    // if(!solver.data()->setHessianMatrix(hessian)) return 1;
    // if(!solver.data()->setGradient(gradient)) return 1;
    // if(!solver.data()->setLinearConstraintsMatrix(linearMatrix)) return 1;
    // if(!solver.data()->setLowerBound(lowerBound)) return 1;
    // if(!solver.data()->setUpperBound(upperBound)) return 1;



    // if (!solver.initSolver()) { // If solver cannot be initialized
    //     throw std::runtime_error("Cannot initialize solver");
    // }
}