/**
 * @file condensed_qp.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */
#include "MPC/condensed_qp.h"
#include <limits>

/**
 * @brief Helper function. Implementing block diagonal matrix
 * 
 * @param blk_mat Eigen::SparseMatrix<double> to be block diagonalized
 * @param arg block argument
 * @param count Number of blocks
 */
static void blkdiag(SparseXd& blk_mat, const MatrixXd& arg, int count) {
    MatrixXd mat = MatrixXd::Zero(arg.rows() * count, arg.cols() * count);
    for (int i = 0; i < count; i++) {
        mat.block(i * arg.rows(), i * arg.cols(), arg.rows(), arg.cols()) = arg;
    }
    blk_mat = mat.sparseView();
}

/**
 * @brief Set the Tau object, sliced from @param ref
 * 
 * @param ref Reference data
 * @param P Prediction horizon
 * @param W Delay horizon
 * @param n_CV Number of controlled variables
 * @param k MPC simulation step
 * @return Eigen::MatrixXd tau
 */
static VectorXd setTau(const MatrixXd& ref, int P, int W, int n_CV, int k) { 
    VectorXd tau = VectorXd::Zero(n_CV * (P - W));
    for (int i = 0; i < n_CV; i++) {
        int start_index = k + W; 
        VectorXd tmp = ref(i, Eigen::seq(start_index, start_index + (P - W) - 1));
        tau.block(i * (P - W), 0, (P - W), 1) = tmp; 
    }
    return tau;
}

/**
 * @brief Extract the k-dependant constraint from constraint vector
 * 
 * @param bound Eigen::VectorXd representing a bound constraint
 * @param fsr FSRModel Finite step response model
 * @param K_inv Eigen::MatrixXd lower triangular matrix
 * @param Gamma Eigen::SparseXd 
 * @param m Number of constraints 
 * @param n Number of optimalization variables
 * @param W 
 */
static void UpdateBounds(VectorXd& bound, FSRModel& fsr, const MatrixXd& K_inv, 
                const SparseXd& Gamma, int m, int a) { 
    // c = [ 0 (a),
    //       K⁽⁻¹⁾ Gamma U(k-N) (a),
    //       Lambda (n_CV * (P-W)),
    //       Lambda (n_CV * (P-W)),
    //       0 (n_CV),
    //       0 (n_CV)]
    VectorXd c = VectorXd::Zero(m);
    int W = fsr.getW();
    VectorXd lambda = fsr.getLambda();
    int size_lambda = lambda.rows();

    c.block(a, 0, a, 1) = K_inv * Gamma * fsr.getUK();
    c.block(2 * a, 0, size_lambda, 1) = lambda;
    c.block(2 * a + size_lambda, 0, size_lambda, 1) = lambda;
    bound -= c; // Subtract k-dependant part
}

/////////////////////////////
/////// COST FUNCTION ///////
/////////////////////////////

void setWeightMatrices(SparseXd& Q_bar, SparseXd& R_bar, const MPCConfig& conf) {
    // Replicate and flatten Q and R matrices: 
    MatrixXd Q_replicate = conf.Q.replicate(1, conf.P - conf.W);
    VectorXd Q_flatten = Q_replicate.reshaped<Eigen::RowMajor>().transpose();
    MatrixXd R_replicate = conf.R.replicate(1, conf.M);
    VectorXd R_flatten = R_replicate.reshaped<Eigen::RowMajor>().transpose();
    MatrixXd Q_quad = Q_flatten.asDiagonal();
    MatrixXd R = R_flatten.asDiagonal();

    Q_bar = Q_quad.sparseView(); // dim(Q_bar) = n_CV * (P-W) x n_CV * (P-W)
    R_bar = R.sparseView(); // dim(R_bar) = n_MV * M x n_MV * M
}

SparseXd setHessianMatrix(const SparseXd& Q_bar, const SparseXd& R_bar, const MatrixXd& theta, int a, int n) {
    MatrixXd g = MatrixXd::Zero(n, n);
    g.block(0, 0, a, a) = 2 * theta.transpose() * Q_bar * theta + 2 * R_bar;
    
    // G = blkdiag(g (axa), 0 (n_CVxn_CV), 0 (n_CVxn_CV)]
    return g.sparseView();
}

void setGradientVector(VectorXd& q, FSRModel& fsr, const SparseXd& Q_bar,
                        const MatrixXd& ref, const MPCConfig& conf, int n, int k) {
    int W = fsr.getW();
    VectorXd tau = setTau(ref, fsr.getP(), W, fsr.getN_CV(), k);
    // q = [2 Theta^T Q_bar (Lambda - tau),
    //      rho_{h},
    //      rho_{l}]
    q.resize(n);
    VectorXd temp = 2 * fsr.getTheta().transpose() * Q_bar * (fsr.getLambda() - tau);
    q << temp, conf.RoH, conf.RoL;
}

/////////////////////////////
/////// CONSTRAINTS /////////
/////////////////////////////

void setConstraintVectors(VectorXd& l, VectorXd& u, FSRModel& fsr, const VectorXd& c_l, const VectorXd& c_u, const MatrixXd& K_inv,
                         const SparseXd& Gamma, int m, int a) {
    // Reset bounds:
    l.block(0, 0, c_l.rows(), 1) = c_l;
    u.block(0, 0, c_u.rows(), 1) = c_u;

    UpdateBounds(l, fsr, K_inv, Gamma, m, a); // Update lower and upper bound
    UpdateBounds(u, fsr, K_inv, Gamma, m, a);
}

SparseXd setConstraintMatrix(const MatrixXd& theta, int m, int n, int a, int n_CV) {
    // A = [ I (axa),           0 (axn_CV),      0 (axn_CV)
    //       K⁽⁻¹⁾ (axa),       0 (axn_CV),      0 (axn_CV)
    //       Theta* (n_CV*Pxa), -I* (n_CV*Pxn_CV), 0 (n_CV*Pxn_CV)
    //       Theta* (n_CV*Pxa),  0 (n_CV*Pxn_CV), I* (n_CV*Pxn_CV)
    //       0 (n_CVxa),        I (n_CVxn_CV),   0 (n_CVxn_CV)
    //       0 (n_CVxa),        0 (n_CVxn_CV),   I (n_CVxn_CV)]; 
    MatrixXd dense = MatrixXd::Zero(m, n); 
    int dim_theta = theta.rows();

    MatrixXd In_cv = MatrixXd::Identity(n_CV, n_CV);
    MatrixXd Itheta = MatrixXd::Identity(dim_theta, n_CV);
    MatrixXd K_inv = setKInv(a);

    // dU, U row
    dense.block(0, 0, a, a) = MatrixXd::Identity(a, a);
    dense.block(a, 0, a, a) = K_inv;

    // Y row
    dense.block(2 * a, 0, dim_theta, a) = theta;
    dense.block(2 * a + dim_theta, 0, dim_theta, a) = theta;
    dense.block(2 * a, a, dim_theta, n_CV) = -Itheta;
    dense.block(2 * a + dim_theta, a + n_CV, dim_theta, n_CV) = Itheta;

    // eta row
    dense.block(2 * a + 2 * dim_theta, a, n_CV, n_CV) = In_cv;
    dense.block(2 * a + 2 * dim_theta + n_CV, a + n_CV, n_CV, n_CV) = In_cv;
    return dense.sparseView();
}

VectorXd ConfigureConstraint(const VectorXd& z_pop, int m, int a, bool upper) {
    VectorXd bound = VectorXd::Zero(m);
    int duuy_size = z_pop.rows(); // = 2 * M * n_MV + (P-W) * n_CV = 2 * a + (P-W) * n_CV

    if (upper) {
        bound.block(0, 0, duuy_size, 1) = z_pop;
        // Set Infinity values
        bound.block(duuy_size, 0, m - duuy_size, 1) = VectorXd::Constant(m - duuy_size, std::numeric_limits<double>::max()); 
    } else {
        // Extract lower Y constraints
        VectorXd lower_y = z_pop(Eigen::seq(2 * a, Eigen::indexing::last)); // lower_y.rows() = (P-W) * n_CV
        // Set -Infinity values
        bound.block(2 * a, 0, lower_y.rows(), 1) = VectorXd::Constant(lower_y.rows(), std::numeric_limits<double>::min());
        bound.block(duuy_size, 0, lower_y.rows(), 1) = lower_y; // Set lower Y
        bound.block(0, 0, 2 * a, 1) = z_pop(Eigen::seq(0, (2 * a)-1)); // Set lower du and u
    }
    return bound;
}

MatrixXd setKInv(int a) { // Return by value
    MatrixXd K_inv = MatrixXd::Constant(a, a, 1);
    return K_inv.triangularView<Eigen::Lower>();
}

SparseXd setGamma(int M, int n_MV) {
    SparseXd gamma;
    MatrixXd gamma_arg = MatrixXd::Zero(M, 1);
    gamma_arg(0, 0) = 1.0;
    blkdiag(gamma, gamma_arg, n_MV);
    return gamma;
}

SparseXd setOmegaU(int M, int n_MV) {
    MatrixXd omega_dense = MatrixXd::Zero(n_MV, n_MV * M);
    for (int i = 0; i < n_MV; i++) {
        omega_dense(i, i * M) = 1;
    }
    return omega_dense.sparseView();
}

VectorXd PopulateConstraints(const VectorXd& c, const MPCConfig& conf, int a, int n_MV, int n_CV) { 
    // z_pop = [ z - Delta U (M * N_MV), 
    //           z - U (M * N_MV),
    //           z - Y (P-W) * N_CV)]
    int size_y = conf.P - conf.W;
    VectorXd z_pop(2 * conf.M * n_MV + size_y * n_CV);

    for (int var = 0; var < 2 * n_MV; var++) { // Assuming same constraining, u, du if n_MV < n_CV
        z_pop.block(var * conf.M, 0, conf.M, 1) = VectorXd::Constant(conf.M, c(var));
    } // Fill n first constraints, du and u, 2 * M * N_MV

    for (int var = 0; var < n_CV; var++) {
        z_pop.block(2 * a + (var * size_y), 0, size_y, 1) = VectorXd::Constant(size_y, c(2 * n_MV + var));
    } // Fill remaining constraints, y, (P-W) * N_CV
    return z_pop;
} 
