/**
 * @file sr_solver.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */
#include "OSQP-Eigen/sr_solver.h"
#include "OSQP-Eigen/condensed_qp.h"
#include "IO/data_objects.h"
#include "model/FSRModel.h"

#include "OsqpEigen/OsqpEigen.h"
#include <Eigen/Eigen>

#include <stdexcept>

void sr_solver(int T, MatrixXd& u_mat, MatrixXd& y_pred, FSRModel& fsr, const MPCConfig& conf, const VectorXd& z_min, 
             const VectorXd& z_max, VectorXd* y_ref) {
    OsqpEigen::Solver solver;
    solver.settings()->setWarmStart(true); // Starts primal and dual variables from previous QP
    solver.settings()->setVerbosity(false); // Disable printing
    // MPC Scenario variables
    int M = fsr.getM();
    int P = fsr.getP();
    int n_MV = fsr.getN_MV();
    int n_CV = fsr.getN_CV();

    // Define QP
    const int n = M * n_MV; // Optimization variables 
    const int m = P * n_CV + 2 * n; // Constraints

    const VectorXd z_max_pop = PopulateConstraints(z_max, m, n);
    const VectorXd z_min_pop = PopulateConstraints(z_min, m, n);

    solver.data()->setNumberOfVariables(n);
    solver.data()->setNumberOfConstraints(m);

    // Define Cost function variables: 
    SparseXd Q_bar; 
    SparseXd R_bar; 
    SparseXd G;
    SparseXd A;

    // Dynamic variables
    VectorXd q;
    VectorXd l;
    VectorXd u; 

    setWeightMatrices(Q_bar, R_bar, conf);
    setHessianMatrix(G, Q_bar, R_bar, fsr);
    setGradientVector(q, fsr, Q_bar, y_ref);
    setConstraintMatrix(A, fsr, m, n);
    setConstraintVectors(l, u, z_min_pop, z_max_pop, fsr, m, n);

    if (!solver.data()->setHessianMatrix(G)) { throw std::runtime_error("Cannot initialize Hessian"); }
    if (!solver.data()->setGradient(q)) { throw std::runtime_error("Cannot initialize Gradient"); }
    if (!solver.data()->setLinearConstraintsMatrix(A)) { throw std::runtime_error("Cannot initialize constraint matrix"); }
    if (!solver.data()->setLowerBound(l)) { throw std::runtime_error("Cannot initialize lower bound"); }
    if (!solver.data()->setUpperBound(u)) { throw std::runtime_error("Cannot initialize upper bound"); }
    if (!solver.initSolver()) { throw std::runtime_error("Cannot initialize solver"); }

    u_mat = MatrixXd::Zero(n_MV, T);
    y_pred = MatrixXd::Zero(n_CV, T);

    SparseXd omega_u;
    setOmegaU(omega_u, M, n_MV);

    for (int k = 0; k < T; k++) {
        // Optimize:
        if (solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) { throw std::runtime_error("Cannot solve problem"); }

        // Claim solution:
        VectorXd z = solver.getSolution();
        VectorXd du = omega_u * z; 

        // Store optimal du and y_pref: Before update!
        u_mat(Eigen::seq(0, n_MV-1), k) = fsr.getUK();
        y_pred(Eigen::seq(0, n_CV-1), k) = fsr.getY(z);

        // Propagate FSR model:
        fsr.UpdateU(du);

        // Update MPC problem:
        setConstraintVectors(l, u, z_min_pop, z_max_pop, fsr, m, n);
        setGradientVector(q, fsr, Q_bar, y_ref); // NB! not updating y_ref
        // Check if bounds are valid:
        if (!solver.updateBounds(l, u)) { throw std::runtime_error("Cannot update bounds"); }
        if (!solver.updateGradient(q)) { throw std::runtime_error("Cannot update bounds"); }
    }
    //fsr.PrintActuation();
}