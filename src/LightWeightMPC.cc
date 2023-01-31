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
#include <iostream>

LightWeightMPC::LightWeightMPC(const string& sce, int T) : sce_{sce}, T_{T} {
    const string sim = "sim_" + sce_;
    const string sys_path = "../data/systems/sr_siso_test.json";
    const string sce_path = "../data/scenarios/sce_" + sce + ".json";
    const string sim_path = "../data/simulations/" + sim + ".json"; 

    // System variables
    std::map<string, int> m_map;
    CVData cvd(T_);
    MVData mvd;

    // Scenario variables:
    MPCConfig conf; //Default initializer
    VectorXd z_max;
    VectorXd z_min; 

    // Parse
    Parse(sce_path, m_map, cvd, mvd, 
        conf, z_min, z_max, T_);
    
    // Model setup
    FSRModel fsr(cvd.getSR(), m_map, conf.P, conf.M, 
                 conf.W, mvd.Inits, cvd.getInits());
    
    MatrixXd u_mat; // Optimized actuation
    MatrixXd y_pred;

    VectorXd ex = cvd.getYRef()[1];
    std::cout << ex.rows() << std::endl;
    
    SRSolver(T_, u_mat, y_pred, fsr, conf, z_min, z_max, cvd.getYRef());

    // // // Serializing: 
    // json write_data;
    // SerializeSimulation(write_data, sim_path, sce, sd, id, 
    //              y_pred, u_mat, z_min, z_max, fsr.getN_CV(), fsr.getN_MV(), T);

}