/**
 * @file FSRModel.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */
#include "model/FSRModel.h"
#include "IO/json_specifiers.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <Eigen/Eigen>

FSRModel::FSRModel(VectorXd** SR, std::map<std::string, int> m_param, int P, int M, int W,
                   const std::vector<double>& init_u, const std::vector<double>& init_y) :
                      P_{P}, M_{M}, W_{W} {
    n_CV_ = m_param[kN_CV];
    n_MV_ = m_param[kN_MV];  
    N_ = m_param[kN];

    theta_ = MatrixXd::Zero(n_CV_*(P-W), n_MV_*M);
    phi_.resize(n_CV_*(P-W), n_MV_*(N_-W_-1));
    psi_.resize(n_CV_*(P-W), n_MV_);
    du_tilde_mat_ = MatrixXd::Zero(n_MV_, N_-W-1);

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

// FSRModel& FSRModel::operator=(const FSRModel& rhs) {
//     n_CV_ = rhs.n_CV_;
//     n_MV_ = rhs.n_MV_;
//     N_ = rhs.N_;
//     P_ = rhs.P_;
//     M_ = rhs.M_;
//     W_ = rhs.W_;

//     u_ = rhs.u_;
//     y_ = rhs.y_;

//     u_K_ = rhs.u_K_;
//     du_tilde_mat_ = rhs.du_tilde_mat_;

//     theta_ = rhs.theta_;
//     phi_ = rhs.phi_;
//     psi_ = rhs.psi_;

//     pp_SR_mat_ = new MatrixXd*[n_CV_];
//     for (int i = 0; i < n_CV_; ++i) {
//         pp_SR_mat_[i] = new MatrixXd[n_MV_];
//     }
//     for (int row = 0; row < n_CV_; row++) {
//         for (int col = 0; col < n_MV_; col++) {
//             pp_SR_mat_[row][col] = rhs.pp_SR_mat_[row][col];
//         }
//     }
//     // Deep copy SR into pp_SR_vec
//     pp_SR_vec_ = new VectorXd*[n_CV_];
//     for (int i = 0; i < n_CV_; ++i) {
//         pp_SR_vec_[i] = new VectorXd[n_MV_];
//     }
//     for (int row = 0; row < n_CV_; row++) {
//         for (int col = 0; col < n_MV_; col++) {
//             pp_SR_vec_[row][col] = rhs.pp_SR_vec_[row][col];
//         }
//     }
//     return *this;
// }

FSRModel::~FSRModel() {
    if (n_CV_ != 0 && n_MV_ != 0) {
        for (int i = 0 ; i < n_CV_; i++) {
            delete[] pp_SR_vec_[i];
            delete[] pp_SR_mat_[i];
        }
        delete[] pp_SR_vec_;
        delete[] pp_SR_mat_;
    }
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
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) { 
            double sn = pp_SR_vec_[i][j](Eigen::last); // S(N)
            
            for (int pad = 0; pad < P_; pad++) {
                // Extract S-coefficients
                VectorXd vec = pp_SR_vec_[i][j](Eigen::seq(W_ + 1 + pad, Eigen::last)); // Accessing [S(W+1+k), ..., S(N)]
                
                VectorXd pad_vec = PadVec(vec, pad, sn); // pad_vec.rows() = N-W-1
                int size = pad_vec.rows(); 
                // Write to phi-matrix
                phi_((i * P_) + pad, Eigen::seq(j * size, (j+1) * size - 1)) = pad_vec(Eigen::seq(0, Eigen::last)).transpose();
            }
        }
    }
}

VectorXd FSRModel::PadVec(VectorXd& vec, int pad, double sn) {
    if (pad == 0) {
        return vec;
    }

    VectorXd sn_vec = VectorXd::Constant(pad, sn);
    VectorXd pad_vec(vec.size() + sn_vec.size());
    pad_vec << 
        vec, sn_vec; // Concatinating two Eigen::VectorXd
    return pad_vec; 
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

void FSRModel::UpdateU(const VectorXd& du) { // du = omega_u * z
    // Updating U(k-1)
    u_K_ += du; 

    // Updating U(n)
    VectorXd du_n = du_tilde_mat_.rightCols(1);
    u_ += du_n;

    // Update du_tilde by left shift, adding the optimized du
    MatrixXd old_du = du_tilde_mat_.leftCols(N_-W_-2);
    du_tilde_mat_.block(0, 0, n_MV_, 1) = du;
    du_tilde_mat_.block(0, 1, n_MV_, N_-W_-2) = old_du;
}

SparseXd FSRModel::getOmegaY() {
    MatrixXd omega_dense = MatrixXd::Zero(n_CV_, n_CV_ * P_);
    for (int i = 0; i < n_CV_; i++) {
        omega_dense(i, i * n_CV_) = 1;
    }
    return omega_dense.sparseView();
}  

// Print functions: 
void FSRModel::PrintTheta() const {
    std::cout << "Theta: " << "(" << theta_.rows() << ", " << theta_.cols() << ")" << std::endl; 
    std::cout << theta_ << std::endl; 
    std::cout << std::endl;
}

void FSRModel::PrintPhi() const {
    std::cout << "Phi : " << "(" << phi_.rows() << ", " << phi_.cols() << ")" << std::endl;
    
    for (int i = 0; i < phi_.rows(); i++) {
        std::cout << "Row = pad: " << i << std::endl;
        //VectorXd vec = phi_(i, Eigen::seq(0, N_-W_-2-1));
        std::cout << phi_(i, Eigen::seq(0, N_-W_-1-1)) << std::endl;
        std::cout << std::endl;
        //VectorXd second = phi_(i, Eigen::seq(N_-W_-2, Eigen::last));
        std::cout << phi_(i, Eigen::seq(N_-W_-1, Eigen::last)) << std::endl;
        std::cout << std::endl;
    }
}

void FSRModel::PrintPsi() const {
    std::cout << "Psi : " << "(" << psi_.rows() << ", " << psi_.cols() << ")" << std::endl;
    std::cout << psi_ << std::endl; 
    std::cout << std::endl;
}

void FSRModel::PrintActuation() const {
    std::cout << "U(k-1):" << std::endl;
    std::cout << u_K_ << std::endl;
    std::cout << std::endl;
    std::cout << "U_tilde:" << std::endl;
    std::cout << du_tilde_mat_ << std::endl;
    std::cout << std::endl;
    std::cout << "U(k-N):" << std::endl;
    std::cout << u_ << std::endl;
}
