/**
 * @file FSRModel.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */
#include "FSRModel.h"
#include <iostream>
#include <vector>

#include <Eigen/Dense>
using MatrixXf = Eigen::MatrixXf;
using VectorXf = Eigen::VectorXf; 

FSRModel::FSRModel(VectorXf** SR, int n_CV, int n_MV, int N, int P, int M, int W,
                   const std::vector<float>& init_u, const std::vector<float>& init_y) :
                     n_CV_{n_CV}, n_MV_{n_MV}, N_{N}, P_{P}, M_{M}, W_{W} {
    theta_ = MatrixXf::Zero(n_CV*(P-W), n_MV*M);
    phi_.resize(n_CV*(P-W), n_MV*(N-W-1));
    azymuth_.resize(n_CV*(P-W), n_MV);
    du_tilde.resize(n_MV * (N-W-1), N-1);
    u = Eigen::VectorXf::Map(init_u.data(), init_u.size());
    y = Eigen::VectorXf::Map(init_y.data(), init_y.size());

    AllocateAndDeepCopy(SR); 
    // Setting matrix member variables
    setSRMatrix();
    setThetaMatrix();
    setPhiMatrix();
    setAzymuth();
}

FSRModel::~FSRModel() {
    for (int i = 0 ; i < n_CV_; i++) {
        delete[] pp_SR_vec_[i];
        delete[] pp_SR_mat_[i];
    }
    delete[] pp_SR_vec_;
    delete[] pp_SR_mat_;
}

void FSRModel::AllocateAndDeepCopy(VectorXf** SR) {
    // Allocate memory
    pp_SR_mat_ = new MatrixXf*[n_CV_];
    for (int i = 0; i < n_CV_; ++i) {
        pp_SR_mat_[i] = new MatrixXf[n_MV_];
    }
    for (int row = 0; row < n_CV_; row++) {
        for (int col = 0; col < n_MV_; col++) {
            pp_SR_mat_[row][col] = MatrixXf::Zero(P_, M_);
        }
    }
    // Deep copy SR into pp_SR_vec
    pp_SR_vec_ = new VectorXf*[n_CV_];
    for (int i = 0; i < n_CV_; ++i) {
        pp_SR_vec_[i] = new VectorXf[n_MV_];
    }
    for (int row = 0; row < n_CV_; row++) {
        for (int col = 0; col < n_MV_; col++) {
            pp_SR_vec_[row][col] = SR[row][col];
        }
    }
}

void FSRModel::setLowerTriangularMatrix(const VectorXf& pred_vec, MatrixXf& S) {
    int n = 0; 
    for (int i = 0; i < M_; i++) {
        for (int j = 0; j < P_-n; j++) {
            S(j+n, i) = pred_vec(j);
        }
        n++;
    }
}

void FSRModel::setSRMatrix() {
    for (int i = 0; i < n_CV_; i++) {
        MatrixXf S = MatrixXf::Zero(P_, M_);
        for (int j = 0; j < n_MV_; j++) {
            setLowerTriangularMatrix(pp_SR_vec_[i][j], S);
            pp_SR_mat_[i][j] = S(Eigen::seq(W_, Eigen::last), Eigen::seq(0, Eigen::last));
        }
    }
}

void FSRModel::setThetaMatrix() {
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) {
            theta_.block(i*(P_-W_), j*M_, P_-W_, M_) = pp_SR_mat_[i][j]; // The elements are already sliced
        }
    }
}

void FSRModel::setPhiMatrix() {
    // NB: Need supervision for padding Sn 
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) { 
            VectorXf vec = pp_SR_vec_[i][j](Eigen::seq(P_-W_+j,Eigen::last));
            if (vec.size() != N_-P_) { // Pad vector
                int pad = vec.size() - (N_-W_-1);
                VectorXf vec = PadVec(vec, pad);
            }
            FillRowPhi(vec, i);
        }
    }
}

VectorXf FSRModel::PadVec(VectorXf& vec, int pad) {
    float num = vec(Eigen::last); // Accessing Sn
    VectorXf num_vec = VectorXf::Constant(pad, num);
    VectorXf padded_vec(vec.size() + num_vec.size());
    padded_vec << vec; // Concatinating two Eigen::VectorXf
    padded_vec << num_vec;
    return padded_vec; 
}

void FSRModel::FillRowPhi(const VectorXf& pad_vec, const int& row) {
    for (int i = 0; i < n_MV_; i++) { // Might need to be tested
        for (int j = row; j < P_; j++) {
            phi_(j, Eigen::seq(i*(N_-P_-1), (i+1)*(N_-P_-1))) = pad_vec(Eigen::seq(0, Eigen::last));
        } 
    }
}

void FSRModel::setAzymuth() {
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) {
            VectorXf vec = VectorXf::Constant(P_-W_, pp_SR_vec_[i][j](Eigen::last));
            azymuth_.block(i*(P_-W_), j, P_-W_, 1) = vec;
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

void FSRModel::PrintAzymuth() {
    std::cout << "Asymuth : " << "(" << azymuth_.rows() << ", " << azymuth_.cols() << ")" << std::endl;
    std::cout << azymuth_ << std::endl; 
    std::cout << std::endl;
}
