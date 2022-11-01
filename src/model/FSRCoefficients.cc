/**
 * @file FSRCoefficients.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "FSRCoefficients.h"
#include <iostream>

#include <Eigen/Dense>

FSRCoefficients::FSRCoefficients(const Eigen::MatrixXf& SR, int n_CV, int n_MV, int N, int P, int M, int W) : SR_{SR}, 
                                 n_CV_{n_CV}, n_MV_{n_MV}, N_{N}, P_{P}, M_{M}, W_{W} {
    // Initializing theta, phi and SR_matrix
    theta_ = Eigen::MatrixXf::Zero(n_CV*(P-W), n_MV*M);
    phi_ = Eigen::MatrixXf::Zero(P, N-P);

    //Allocate memory
    pp_SR_ = new Eigen::MatrixXf*[n_CV];
    for (int i = 0; i < n_CV; ++i) {
        pp_SR_[i] = new Eigen::MatrixXf[n_MV];
    }

    for (int row = 0; row < n_CV; row++) {
        for (int col = 0; col < n_MV; col++) {
            pp_SR_[row][col] = Eigen::MatrixXf::Zero(P, M);
        }
    }
    setSRMatrix();
}

FSRCoefficients::~FSRCoefficients() {
    for (int i = 0 ; i < n_CV_ ; i++ ) {
        delete[] pp_SR_[i];
        delete[] pp_SR_;
    }
}

void FSRCoefficients::GenerateLowerTriangularMatrix(const Eigen::VectorXf& vec, Eigen::MatrixXf& S) {
    int n = 0; 
    for (int i = 0; i < M_; i++) {
        for (int j = 0; j < P_-n; j++) {
            S(j+n, i) = vec(j);
        }
        n++;
    }
}

void FSRCoefficients::SelectPredictionVec(Eigen::MatrixXf& S) {
    for (int i = 0; i < n_CV_; i++) {
        Eigen::VectorXf vec = Eigen::VectorXf::Zero(P_);
        for (int j = 0; j < n_MV_*N_; j = j + N_) {
            vec(Eigen::seq(0, Eigen::last)) = SR_(i,Eigen::seq(0, P_-1));
            GenerateLowerTriangularMatrix(vec, S);
        }
    }
}

void FSRCoefficients::setSRMatrix() {
    for (int i = 0; i < n_MV_; i++) {
        Eigen::MatrixXf S = Eigen::MatrixXf::Zero(P_, M_);
        for (int j = 0; j < n_CV_; j++) {
            SelectPredictionVec(S);
            pp_SR_[i][j] = S(Eigen::seq(W_, Eigen::last), Eigen::seq(0, Eigen::last));
        }
    }
}

void FSRCoefficients::setThetaMatrix() {
    for (int i = 0; i < n_MV_; i++) {
        for (int j = 0; j < n_CV_; j++) {
            //pp_SR_[i][j]
        }
    }
}

void FSRCoefficients::PrintPPSR(int i, int j) {
    std::cout << pp_SR_[i][j] << std::endl;
}