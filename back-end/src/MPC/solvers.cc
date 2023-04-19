/**
 * @file solvers.cc
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Released under the terms of the BSD 3-Clause License
 * 
 */
#include "MPC/solvers.h"
#include "MPC/condensed_qp.h"
#include "OsqpEigen/OsqpEigen.h"

#include <stdexcept>

using SparseXd = Eigen::SparseMatrix<double>; 

void SRSolver(int T, MatrixXd& u_mat, MatrixXd& y_pred, FSRModel& fsr, const MPCConfig& conf, const VectorXd& z_min, 
             const VectorXd& z_max, VectorXd* y_ref) {
    // Setup solver:
    OsqpEigen::Solver solver;
    solver.settings()->setWarmStart(true); // Starts primal and dual variables from previous QP
    solver.settings()->setVerbosity(false); // Disable printing

    // MPC Scenario variables:
    int P = fsr.getP(); // Prediction Horizon
    int M = fsr.getM(); // Control Horizon
    int n_MV = fsr.getN_MV();
    int n_CV = fsr.getN_CV();

    // Define QP:
    const int n = M * n_MV + 2 * n_CV; // #Optimization variables 
    const int m = 2 * M * n_MV + 4 * P * n_CV; // #Constraints
    const int a = M * n_MV; // dim(du)
   
    const VectorXd z_max_pop = PopulateConstraints(z_max, a, n_MV, n_CV, M, P);
    const VectorXd z_min_pop = PopulateConstraints(z_min, a, n_MV, n_CV, M, P);

    solver.data()->setNumberOfVariables(n);
    solver.data()->setNumberOfConstraints(m);

    // Define Cost function variables: 
    SparseXd Q_bar; 
    SparseXd R_bar; 
    SparseXd H;
    SparseXd A;

    // Dynamic variables:
    VectorXd q;
    VectorXd l;
    VectorXd u; 

    setWeightMatrices(Q_bar, R_bar, conf);
    setHessianMatrix(H, Q_bar, R_bar, fsr, a, n);

    setGradientVector(q, fsr, Q_bar, y_ref, conf, n, 0); // Initial gradient
    setConstraintMatrix(A, fsr, m, n, a);
    setConstraintVectors(l, u, z_min_pop, z_max_pop, fsr, m, a);

    if (!solver.data()->setHessianMatrix(H)) { throw std::runtime_error("Cannot initialize Hessian"); }
    if (!solver.data()->setGradient(q)) { throw std::runtime_error("Cannot initialize Gradient"); }
    if (!solver.data()->setLinearConstraintsMatrix(A)) { throw std::runtime_error("Cannot initialize constraint matrix"); }
    if (!solver.data()->setLowerBound(l)) { throw std::runtime_error("Cannot initialize lower bound"); }
    if (!solver.data()->setUpperBound(u)) { throw std::runtime_error("Cannot initialize upper bound"); }
    if (!solver.initSolver()) { throw std::runtime_error("Cannot initialize solver"); }

    u_mat = MatrixXd::Zero(n_MV, T);
    y_pred = MatrixXd::Zero(n_CV, T);

    SparseXd omega_u;
    setOmegaU(omega_u, M, n_MV);

    // MPC loop:
    for (int k = 0; k < T; k++) { 
        // Optimize:
        if (solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) { throw std::runtime_error("Cannot solve problem"); }

        // Claim solution:
        VectorXd z_st = solver.getSolution(); // [dU, eta_h, eta_l]
        VectorXd z = z_st(Eigen::seq(0, a - 1)); // [dU]
        VectorXd du = omega_u * z; 

        // Store optimal du and y_pref: Before update!
        u_mat.col(k) = fsr.getUK();
        y_pred.col(k) = fsr.getY(z);

        // Propagate FSR model:
        fsr.UpdateU(du);

        // Update MPC problem:
        setConstraintVectors(l, u, z_min_pop, z_max_pop, fsr, m, a);
        setGradientVector(q, fsr, Q_bar, y_ref, conf, n, k); 

        // Check if bounds are valid:
        if (!solver.updateBounds(l, u)) { throw std::runtime_error("Cannot update bounds"); }
        if (!solver.updateGradient(q)) { throw std::runtime_error("Cannot update gradient"); }    
    }
}