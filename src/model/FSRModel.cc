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
using MatrixXd = Eigen::MatrixXd;
using VectorXd = Eigen::VectorXd; 

FSRModel::FSRModel(VectorXd** SR, int n_CV, int n_MV, int N, int P, int M, int W,
                   const std::vector<double>& init_u, const std::vector<double>& init_y) :
                     n_CV_{n_CV}, n_MV_{n_MV}, N_{N}, P_{P}, M_{M}, W_{W} {
    theta_ = MatrixXd::Zero(n_CV*(P-W), n_MV*M);
    phi_.resize(n_CV*(P-W), n_MV*(N-W-1));
    psi_.resize(n_CV*(P-W), n_MV);
    du_tilde_mat_ = MatrixXd::Zero(n_MV, N-W-1);

    u_K_ = VectorXd::Map(init_u.data(), init_u.size());
    u_ = VectorXd::Map(init_u.data(), init_u.size());
    y_ = VectorXd::Map(init_y.data(), init_y.size());

    AllocateAndDeepCopy(SR); 
    // Setting matrix member variables
    setSRMatrix();
    setThetaMatrix();
    setPhiMatrix();
    setPsi();
}

FSRModel::~FSRModel() {
    for (int i = 0 ; i < n_CV_; i++) {
        delete[] pp_SR_vec_[i];
        delete[] pp_SR_mat_[i];
    }
    delete[] pp_SR_vec_;
    delete[] pp_SR_mat_;
}

void FSRModel::AllocateAndDeepCopy(VectorXd** SR) {
    // Allocate memory
    pp_SR_mat_ = new MatrixXd*[n_CV_];
    for (int i = 0; i < n_CV_; ++i) {
        pp_SR_mat_[i] = new MatrixXd[n_MV_];
    }
    for (int row = 0; row < n_CV_; row++) {
        for (int col = 0; col < n_MV_; col++) {
            pp_SR_mat_[row][col] = MatrixXd::Zero(P_, M_);
        }
    }
    // Deep copy SR into pp_SR_vec
    pp_SR_vec_ = new VectorXd*[n_CV_];
    for (int i = 0; i < n_CV_; ++i) {
        pp_SR_vec_[i] = new VectorXd[n_MV_];
    }
    for (int row = 0; row < n_CV_; row++) {
        for (int col = 0; col < n_MV_; col++) {
            pp_SR_vec_[row][col] = SR[row][col];
        }
    }
}

void FSRModel::setLowerTriangularMatrix(const VectorXd& pred_vec, MatrixXd& S) {
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
        MatrixXd S = MatrixXd::Zero(P_, M_);
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
            VectorXd vec = pp_SR_vec_[i][j](Eigen::seq(P_-W_+j,Eigen::last));
            if (vec.size() != N_-P_) { // Pad vector
                int pad = vec.size() - (N_-W_-1);
                VectorXd vec = PadVec(vec, pad);
            }
            FillRowPhi(vec, i);
        }
    }
}

VectorXd FSRModel::PadVec(VectorXd& vec, int pad) {
    double num = vec(Eigen::last); // Accessing Sn
    VectorXd num_vec = VectorXd::Constant(pad, num);
    VectorXd padded_vec(vec.size() + num_vec.size());
    padded_vec << vec; // Concatinating two Eigen::VectorXd
    padded_vec << num_vec;
    return padded_vec; 
}

void FSRModel::FillRowPhi(const VectorXd& pad_vec, const int& row) {
    for (int i = 0; i < n_MV_; i++) { // Might need to be tested
        for (int j = row; j < P_; j++) {
            phi_(j, Eigen::seq(i*(N_-P_-1), (i+1)*(N_-P_-1))) = pad_vec(Eigen::seq(0, Eigen::last));
        } 
    }
}

void FSRModel::setPsi() {
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) {
            VectorXd vec = VectorXd::Constant(P_-W_, pp_SR_vec_[i][j](Eigen::last));
            psi_.block(i*(P_-W_), j, P_-W_, 1) = vec;
        }
    }
}

VectorXd FSRModel::getDuTilde() {
    VectorXd du_tilde = VectorXd::Zero(n_MV_*(N_-W_-1));
    for (int i = 0; i < n_MV_; i++) {
        du_tilde.block(i * (N_-W_-1), 0, N_-W_-1, 1) = du_tilde_mat_.row(i).transpose();
    }
    return du_tilde;
}

void FSRModel::UpdateU(const VectorXd& du) { // du = delta * z
    u_K_ += du; 
    // Updating U(n)
    VectorXd du_n = du_tilde_mat_.rightCols(1);
    u_ += du_n;
    // Update du_tilde by left shift, adding the optimized du
    MatrixXd old_du = du_tilde_mat_.leftCols(N_-W_-2);
    du_tilde_mat_.block(0, 0, n_MV_, 1) = du;
    du_tilde_mat_.block(0, 1, n_MV_, N_-W_-2) = old_du;
}

// Print functions: 
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

void FSRModel::PrintPsi() {
    std::cout << "Psi : " << "(" << psi_.rows() << ", " << psi_.cols() << ")" << std::endl;
    std::cout << psi_ << std::endl; 
    std::cout << std::endl;
}

void FSRModel::PrintActuation() {
    std::cout << "U(k-1):" << std::endl;
    std::cout << u_K_ << std::endl;
    std::cout << std::endl;
    std::cout << "U_tilde:" << std::endl;
    std::cout << du_tilde_mat_ << std::endl;
    std::cout << std::endl;
    std::cout << "U(k-N):" << std::endl;
    std::cout << u_ << std::endl;
}
