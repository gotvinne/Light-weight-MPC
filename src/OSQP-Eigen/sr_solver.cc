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

void setHessianMatrix(Eigen::MatrixXf& hessian, const Eigen::MatrixXf& theta, const Eigen::MatrixXf& Q_bar, 
                        const Eigen::MatrixXf& R_bar, int n_MV, int M) {
    hessian.resize(M*n_MV, M*n_MV);
    hessian = 2*theta.transpose()*Q_bar*theta + 2*R_bar;
}

void setKmatrix(Eigen::MatrixXf& blk_mat, int M, int n_MV) {
    Eigen::MatrixXf K = Eigen::MatrixXf::Zero(M, M);
    std::array<float, 2> arr = {1, -1};
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < arr.size(); j++) {
            K(i+j, i) = arr[j];
        }
    }
    blk_mat.resize(M * n_MV, M * n_MV);
    blkdiag(blk_mat, K, n_MV);
}

void blkdiag(Eigen::MatrixXf blk_mat, const Eigen::MatrixXf& arg, int count) {
    Eigen::MatrixXf bdm = Eigen::MatrixXf::Zero(arg.rows() * count, arg.cols() * count);
    for (int i = 0; i < count; i++) {
        bdm.block(i * arg.rows(), i * arg.cols(), arg.rows(), arg.cols()) = arg;
    }
}

void sr_solver(const int& T, const FSRModel& fsr, const MPCConfig& conf) {

    Eigen::MatrixXf Q_bar; 
    Eigen::MatrixXf R_bar; 
    Eigen::MatrixXf hessian;

    setWeightMatrices(Q_bar, R_bar, conf);
    setHessianMatrix(hessian, fsr.getTheta(), Q_bar, R_bar, conf.M, conf.M);

    OsqpEigen::Solver solver;
    solver.settings()->setWarmStart(true); // Starts primal and dual variables from previous QP

    // Define QP
    //solver.data()->setNumberOfVariables();
    //solver.data()->setNumberOfConstraints(model_param[k]);


    if (!solver.initSolver()) { // If solver cannot be initialized
        throw std::runtime_error("Cannot initialize solver");
    }
}