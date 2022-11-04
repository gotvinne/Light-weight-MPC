/**
 * @file FSRModel.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "FSRModel.h"
#include <iostream>

#include <Eigen/Dense>

FSRModel::FSRModel(Eigen::VectorXf** SR, int n_CV, int n_MV, int N, int P, int M, int W) :
                     n_CV_{n_CV}, n_MV_{n_MV}, N_{N}, P_{P}, M_{M}, W_{W} {
    // Initializing theta, phi and SR_matrix
    theta_ = Eigen::MatrixXf::Zero(n_CV*(P-W), n_MV*M);
    phi_ = Eigen::MatrixXf::Zero(n_CV*(P-W), n_MV*(N-(P-W)));

    // Allocate memory
    pp_SR_mat_ = new Eigen::MatrixXf*[n_CV];
    for (int i = 0; i < n_CV; ++i) {
        pp_SR_mat_[i] = new Eigen::MatrixXf[n_MV];
    }

    for (int row = 0; row < n_CV; row++) {
        for (int col = 0; col < n_MV; col++) {
            pp_SR_mat_[row][col] = Eigen::MatrixXf::Zero(P, M);
        }
    }

    // Deep copy
    pp_SR_vec_ = new Eigen::VectorXf*[n_CV];
    for (int i = 0; i < n_CV; ++i) {
        pp_SR_vec_[i] = new Eigen::VectorXf[n_MV];
    }

    for (int row = 0; row < n_CV; row++) {
        for (int col = 0; col < n_MV; col++) {
            pp_SR_vec_[row][col] = SR[row][col];
        }
    }

    // Setting matrix member variables
    setSRMatrix();
    setThetaMatrix();
    setPhiMatrix();
}

FSRModel::~FSRModel() {
    for (int i = 0 ; i < n_CV_; i++) {
        delete[] pp_SR_vec_[i];
        delete[] pp_SR_mat_[i];
    }
    delete[] pp_SR_vec_;
    delete[] pp_SR_mat_;
}

void FSRModel::GenerateLowerTriangularMatrix(const Eigen::VectorXf& vec, Eigen::MatrixXf& S) {
    int n = 0; 
    for (int i = 0; i < M_; i++) {
        for (int j = 0; j < P_-n; j++) {
            S(j+n, i) = vec(j);
        }
        n++;
    }
}

void FSRModel::setSRMatrix() {
    for (int i = 0; i < n_CV_; i++) {
        Eigen::MatrixXf S = Eigen::MatrixXf::Zero(P_, M_);
        for (int j = 0; j < n_MV_; j++) {
            GenerateLowerTriangularMatrix(pp_SR_vec_[i][j], S);
            pp_SR_mat_[i][j] = S(Eigen::seq(W_, Eigen::last), Eigen::seq(0, Eigen::last));
        }
    }
}

void FSRModel::setThetaMatrix() {
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) {
            theta_.block(i*(P_-W_), j*M_, P_-W_, M_) = pp_SR_mat_[i][j];
        }
    }
}

void FSRModel::setPhiMatrix() {
    // NB: Need supervision for padding Sn 
    for (int i = 0; i < n_CV_; i++) {
        Eigen::VectorXf vec = Eigen::VectorXf::Zero(N_-P_);
        for (int j = 0; j < n_MV_; j++) { 
            // Pad vector
            //if (vec.size() !=  )
            vec = pp_SR_vec_[i][j](Eigen::seq(P_-W_+j,Eigen::last)); 
            FillPhi(vec, i);
        }
    }
}

void FSRModel::FillPhi(const Eigen::VectorXf& vec, const int& row) {
    for (int i = 0; i < n_MV_; i++) {
        for (int j = row; j < P_; j++) {
            phi_(j, Eigen::seq(i*(N_-P_-1), (i+1)*(N_-P_-1))) = vec(Eigen::seq(0, Eigen::last));
        } 
    }
}


// Print functions: 
void FSRModel::PrintPPSR(int i, int j) {
    std::cout << "SISO SRC matrix: " << "(" << P_-W_ << ", " << M_ << ")" << std::endl; 
    std::cout << pp_SR_mat_[i][j] << std::endl;
    std::cout << std::endl;
}

void FSRModel::PrintTheta() {
    std::cout << "Theta: " << "(" << theta_.rows() << ", " << theta_.cols() << ")" << std::endl; 
    std::cout << theta_ << std::endl; 
    std::cout << std::endl;
}

void FSRModel::PrintPhi() {
    std::cout << "Phi : " << "(" << phi_.rows() << ", " << phi_.cols() << ")" << std::endl;
    std::cout << phi_ << std::endl; 
    std::cout << std::endl;
}