/**
 * @file LightWeightMPC.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "LightWeightMPC.h"
#include "OSQP-Eigen/condensed_qp.h"
#include "IO/json_specifiers.h"

#include <OsqpEigen/OsqpEigen.h>
#include <Eigen/Dense>
#include <iostream>
#include <vector>

/**
 * @brief Allocates and initialises reference
 * 
 * @param y_ref 
 * @param ref_vec 
 * @param T 
 */
static VectorXd* AllocateReference(const std::vector<double>& ref_vec, int T, int P) { // Cannot reinitialize pointer via pass-by-pointer
    VectorXd* y_ref = new VectorXd[int(ref_vec.size())];

    for (int i = 0; i < int(ref_vec.size()); i++) {
        y_ref[i] = VectorXd::Constant(T + P, ref_vec.at(i)); // Takes predictions into account!
    }
    return y_ref;
}

/**
 * @brief Solving the condensed optimalization problem using OSQP-Eigen
 * 
 * @param T MPC horizon
 * @param u_mat Optimized u, filled by reference
 * @param y_pred Predicted y, filled by reference
 * @param fsr FSRModel, finite step response model 
 * @param conf MPC configuration
 * @param z_min lower constraint vector
 * @param z_max upper constraint vector
 * @param y_ref Output reference data
 */
static void SRSolver(int T, MatrixXd& u_mat, MatrixXd& y_pred, FSRModel& fsr, const MPCConfig& conf, const VectorXd& z_min, 
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
    const int n = M * n_MV; // #Optimization variables 
    const int m = P * n_CV + 2 * n; // #Constraints
   
    const VectorXd z_max_pop = PopulateConstraints(z_max, m, n, n_MV, n_CV, M, P);
    const VectorXd z_min_pop = PopulateConstraints(z_min, m, n, n_MV, n_CV, M, P);

    solver.data()->setNumberOfVariables(n);
    solver.data()->setNumberOfConstraints(m);

    // Define Cost function variables: 
    SparseXd Q_bar; 
    SparseXd R_bar; 
    SparseXd G;
    SparseXd A;

    // Dynamic variables:
    VectorXd q;
    VectorXd l;
    VectorXd u; 

    setWeightMatrices(Q_bar, R_bar, conf);
    setHessianMatrix(G, Q_bar, R_bar, fsr);

    setGradientVector(q, fsr, Q_bar, y_ref, 0); // Initial gradient
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

    // MPC loop:
    for (int k = 0; k < T; k++) { 
        // Optimize:
        if (solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) { throw std::runtime_error("Cannot solve problem"); }

        // Claim solution:
        VectorXd z = solver.getSolution();
        VectorXd du = omega_u * z; 

        // Store optimal du and y_pref: Before update!
        u_mat(Eigen::seq(0, Eigen::last), k) = fsr.getUK();
        y_pred(Eigen::seq(0, Eigen::last), k) = fsr.getY(z);

        // Propagate FSR model:
        fsr.UpdateU(du);

        // Update MPC problem:
        setConstraintVectors(l, u, z_min_pop, z_max_pop, fsr, m, n);
        setGradientVector(q, fsr, Q_bar, y_ref, k); 

        // Check if bounds are valid:
        if (!solver.updateBounds(l, u)) { throw std::runtime_error("Cannot update bounds"); }
        if (!solver.updateGradient(q)) { throw std::runtime_error("Cannot update gradient"); }    
    }
    //fsr.PrintActuation();
}

void LightWeightMPC(const string& sce, const std::vector<double>& ref_vec, bool new_sim, int T) {
    const string sim = "sim_" + sce;
    const string sce_path = "../data/scenarios/sce_" + sce + ".json";
    const string sim_path = "../data/simulations/" + sim + ".json";

    // System variables:
    CVData cvd; 
    MVData mvd;
    std::map<string, int> m_map;

    // Scenario variables:
    VectorXd z_min; /** Lower constraint vector */
    VectorXd z_max; /** Upper constraint vector */
    MPCConfig conf; /** MPC configuration */

    // Parse information:
    if (new_sim) {
        ParseNew(sce_path, m_map, cvd, mvd, conf, z_min, z_max);
    } else {
        MatrixXd du_tilde; 
        Parse(sce_path, sim_path, m_map, cvd, mvd, conf, z_min, z_max, du_tilde);
    }
    
    // Select dynamical model: 
    FSRModel fsr(cvd.getSR(), m_map, conf.P, conf.M, conf.W, mvd.Inits, cvd.getInits());
    
    // MPC variables:
    MatrixXd u_mat; /** Optimized actuation, (n_MV, T) */
    MatrixXd y_pred; /** Predicted output (n_CV, T)*/

    // Reference: 
    if (int(ref_vec.size()) != m_map[kN_CV]) {
        throw std::invalid_argument("Number of references do not coincide with constrained variables");
    }
    VectorXd* y_ref = AllocateReference(ref_vec, T, conf.P);

    // Solver: 
    try {
        SRSolver(T, u_mat, y_pred, fsr, conf, z_min, z_max, y_ref);
        delete[] y_ref;
    }
    catch(std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    // Serializing: 
    json write_data;
    SerializeSimulation(write_data, sim_path, sce, cvd, mvd, 
               y_pred, u_mat, z_min, z_max, fsr, T);

}