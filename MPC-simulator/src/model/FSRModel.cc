/**
 * @file FSRModel.cc
 * @author Geir Ola Tvinnereim
 * @copyright Released under the terms of the BSD 3-Clause License
 * @date 2022
 */
#include "model/FSRModel.h"
#include "IO/json_specifiers.h"

FSRModel::FSRModel(VectorXd** SR, std::map<string, int> m_param, const MPCConfig& conf,
                   const std::vector<double>& init_u, const std::vector<double>& init_y) :
                      P_{conf.P}, M_{conf.M}, W_{conf.W} {
    n_CV_ = m_param[kN_CV];
    n_MV_ = m_param[kN_MV];  
    N_ = m_param[kN];

    u_K_ = VectorXd::Map(init_u.data(), init_u.size());
    u_ = VectorXd::Map(init_u.data(), init_u.size());
    y_ = setInitY(init_y, P_ - W_);

    AllocateAndDeepCopy(SR); 
    // Setting matrix member variables
    setSRMatrix();
    theta_ = getThetaMatrix(W_);
    phi_ = getPhiMatrix(W_);
    psi_ = getPsi(W_);

    B_ = VectorXd::Zero((P_ - W_) * n_CV_);
    du_tilde_mat_ = MatrixXd::Zero(n_MV_, N_-W_-1);  
}

FSRModel::FSRModel(VectorXd** SR, std::map<std::string, int> m_param, const std::vector<double>& init_u, 
            const std::vector<double>& init_y) : P_{1}, M_{1}, W_{0} {
    n_CV_ = m_param[kN_CV];
    n_MV_ = m_param[kN_MV];  
    N_ = m_param[kN];

    u_K_ = VectorXd::Map(init_u.data(), init_u.size()); 
    u_ = VectorXd::Map(init_u.data(), init_u.size());
    y_ = VectorXd::Map(init_y.data(), init_y.size());

    AllocateAndDeepCopy(SR); 
    // set FSRM matrix variables
    setSRMatrix();
    theta_ = getThetaMatrix(W_);
    phi_ = getPhiMatrix(W_);
    psi_ = getPsi(W_);

    B_ = VectorXd::Zero((P_ - W_) * n_CV_);
    du_tilde_mat_ = MatrixXd::Zero(n_MV_, N_-W_-1); 
}    

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
    // S = [[ s1, 0, ..., 0 
    //        s2, s1, 0,  . 
    //         ., . ,  ., . 
    //         sM, sM-1, ..., s1
    //          ., ., ., . 
    //         sP, sP-1, ..., sP-M]] (7b) in Light-weight MPC thesis
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
            pp_SR_mat_[i][j] = S;
        }
    }
}

MatrixXd FSRModel::getThetaMatrix(int W) {
    MatrixXd tmp_theta = MatrixXd::Zero(n_CV_*(P_-W), n_MV_*M_);
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) {
            tmp_theta.block(i*(P_-W), j*M_, P_-W, M_) = pp_SR_mat_[i][j](Eigen::seq(W, Eigen::indexing::last), Eigen::seq(0, Eigen::indexing::last)); 
        }
    }
    return tmp_theta; 
}

MatrixXd FSRModel::getPhiMatrix(int W) {
    MatrixXd tmp_phi = MatrixXd::Zero(n_CV_*(P_-W), n_MV_*(N_-W-1));
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) { 
            double sn = pp_SR_vec_[i][j](Eigen::indexing::last); // S(N)
            for (int pad = 0; pad < (P_-W); pad++) {
                // Slicing prior s-coefficients
                VectorXd vec = pp_SR_vec_[i][j](Eigen::seq(W + pad, Eigen::indexing::last - 1)); // Accessing [S(W+k), ..., S(N-1)]
                VectorXd pad_vec = PadVec(vec, pad, sn); // pad_vec.rows() = N-W-1
                int size = pad_vec.rows(); 
                
                // Write to phi-matrix
                tmp_phi((i * (P_-W)) + pad, Eigen::seq(j * size, (j+1) * size - 1)) = pad_vec.transpose();
            }
        }
    }
    return tmp_phi;
}

VectorXd FSRModel::PadVec(VectorXd& vec, int pad, double sn) {
    // Pad vectors with pad number of sn. 
    VectorXd sn_vec = VectorXd::Constant(pad, sn);
    VectorXd pad_vec(vec.size() + sn_vec.size());
    pad_vec << 
        vec, sn_vec; // Concatinating two Eigen::VectorXd
    return pad_vec; 
}

MatrixXd FSRModel::getPsi(int W) {
    MatrixXd tmp_psi = MatrixXd::Zero(n_CV_*(P_-W), n_MV_);
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) {
            VectorXd vec = VectorXd::Constant(P_-W, pp_SR_vec_[i][j](Eigen::indexing::last));
            tmp_psi.block(i*(P_-W), j, P_-W, 1) = vec;
        }
    }
    return tmp_psi;
}

VectorXd FSRModel::getDuTilde() const { // Flattning du_tilde_mat, dependant on W
    VectorXd du_tilde = VectorXd::Zero(n_MV_*(N_-W_-1));
    MatrixXd du_tilde_sliced = du_tilde_mat_.block(0, 0, n_CV_, N_-W_-1);
    for (int i = 0; i < n_MV_; i++) {
        du_tilde.block(i * (N_-W_-1), 0, N_-W_-1, 1) = du_tilde_sliced.row(i).transpose();
    }
    return du_tilde;
}

void FSRModel::UpdateU(const VectorXd& du) { // du = omega_u * z
    // Updating U(k-1)
    u_K_ += du; 
    // Updating U(n)
    VectorXd du_n = du_tilde_mat_.col(N_-2-W_); // Access last - W elem 
    u_ += du_n;
    // Update du_tilde by left shift, adding the optimized du
    MatrixXd old_du = du_tilde_mat_.leftCols(N_-2-W_);
    du_tilde_mat_.block(0, 0, n_MV_, 1) = du;
    du_tilde_mat_.block(0, 1, n_MV_, N_-2-W_) = old_du;
}

SparseXd FSRModel::getOmegaY() const {
    MatrixXd omega_dense = MatrixXd::Zero(n_CV_, n_CV_ * P_);
    for (int i = 0; i < n_CV_; i++) {
        omega_dense(i, i * P_) = 1; 
    }
    return omega_dense.sparseView();
}  

VectorXd FSRModel::setInitY(std::vector<double> init_y, int predictions) {
    VectorXd y_init = VectorXd::Zero(n_CV_ * predictions);
    for (int cv = 0; cv < n_CV_; cv++) {
        y_init.block(cv * predictions, 0, predictions, 1) = VectorXd::Constant(predictions, init_y[cv]);
    }
    return y_init;
}

void FSRModel::setDuTildeMat(const MatrixXd& mat) { 
    // Update u_
    for (int i = 0; i < mat.cols(); i++) {
        VectorXd vec = mat.col(i);
        u_ -= vec;
    }
    du_tilde_mat_ = mat.block(0, 0, n_CV_, N_-1-W_); 
}
