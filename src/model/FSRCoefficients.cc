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
    phi_ = Eigen::MatrixXf::Zero(n_CV*(P-W), n_MV*(N-(P-W)));

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
    setThetaMatrix();
    setPhiMatrix();
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
        for (int j = 0; j < n_MV_*N_; j = j + N_) { // Step N 
            vec(Eigen::seq(0, Eigen::last)) = SR_(i,Eigen::seq(j, j+P_-1));
            GenerateLowerTriangularMatrix(vec, S);
        }
    }
}

void FSRCoefficients::setSRMatrix() {
    for (int i = 0; i < n_CV_; i++) {
        Eigen::MatrixXf S = Eigen::MatrixXf::Zero(P_, M_);
        for (int j = 0; j < n_MV_; j++) {
            SelectPredictionVec(S);
            pp_SR_[i][j] = S(Eigen::seq(W_, Eigen::last), Eigen::seq(0, Eigen::last));
        }
    }
}

void FSRCoefficients::setThetaMatrix() {
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) {
            FillTheta(pp_SR_[i][j], i, j);
        }
    }
}

void FSRCoefficients::FillTheta(const Eigen::MatrixXf& S, const int& i, const int& j) {
    for (int k = j * M_; k < (j+1)*M_; k++) {
        for (int l = i*(P_-W_); l < (i+1)*(P_-W_); l++) {
            theta_(l, k) = S(l, k);
        }
    }
}

void FSRCoefficients::setPhiMatrix() {
    SelectPastVec();
}

void FSRCoefficients::SelectPastVec() {
    for (int i = 0; i < n_CV_; i++) {
        Eigen::VectorXf vec = Eigen::VectorXf::Zero(N_-P_);
        for (int j = 0; j < n_MV_*N_; j = j + N_) { // Step N
            vec(Eigen::seq(0, Eigen::last)) = SR_(i,Eigen::seq(j+P_, j+N_-1));
            FillPhi(vec, i);
        }
    }
}

void FSRCoefficients::FillPhi(const Eigen::VectorXf& vec, const int& row) {
    for (int i = 0; i < n_MV_; i++) {
        phi_(row, Eigen::seq(i*(N_-P_-1), (i+1)*(N_-P_-1))) = vec(Eigen::seq(0, Eigen::last));
    }
}

void FSRCoefficients::PrintPPSR(int i, int j) {
    std::cout << "SISO SRC matrix: " << "(" << P_-W_ << ", " << M_ << ")" << std::endl; 
    std::cout << pp_SR_[i][j] << std::endl;
    std::cout << std::endl;
}

void FSRCoefficients::PrintTheta() {
    std::cout << "Theta: " << "(" << theta_.rows() << ", " << theta_.cols() << ")" << std::endl; 
    std::cout << theta_ << std::endl; 
    std::cout << std::endl;
}

void FSRCoefficients::PrintSR() {
    std::cout << "SR matrix: " << "(" << SR_.rows() << ", " << SR_.cols() << ")" << std::endl;
    std::cout << SR_ << std::endl; 
    std::cout << std::endl;
}

void FSRCoefficients::PrintPhi() {
    std::cout << "Phi : " << "(" << phi_.rows() << ", " << phi_.cols() << ")" << std::endl;
    std::cout << phi_ << std::endl; 
    std::cout << std::endl;
}