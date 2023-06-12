/**
 * @file simulations.cc
 * @author Geir Ola Tvinnereim
 * @brief Script for simulation functionalities
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Released under the terms of the BSD 3-Clause License
 * 
 */
#include "simulations.h"
#include "MPC/solvers.h"

#include "IO/json_specifiers.h"
#include "IO/data_objects.h"
#include "IO/parse.h"
#include "IO/serialize.h"
#include "model/FSRModel.h"

#include <iostream>
#include <vector>

#include <Eigen/Dense>
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;

/**
 * @brief set Reference variable based on string
 * 
 * @param ref_str reference string
 * @param T MPC horizon
 * @param P Prediction horizon
 * @param n_CV number of controlled variables
 * @return MatrixXd reference variable
 */
static MatrixXd setRef(const string& ref_str, int T, int P, int n_CV) {
    // ref = R^(n_CV x T + P + 1)
    // Split string to std::vector
    std::vector<double> ref_vec = ParseRefString(ref_str); 
    
    int size = int(ref_vec.size());
    MatrixXd ref = MatrixXd::Zero(size, T + P + 1);
    if (size != n_CV) {
        throw std::invalid_argument("Number of references do not coincide with number of n_CV, expected: " + std::to_string(n_CV));
    }
    // Fill ref matrix:
    for (int i = 0; i < size; i++) {
        ref.row(i) = VectorXd::Constant(T + P + 1, ref_vec.at(i)); // Take predictions and y0 into account
    }
    return ref;
}

void MPCSimFSRM(const string& sys, const string& ref_vec, bool new_sim, int T) {
    // Mapping to Data folder
    const string sim = "sim_" + sys;
    const string sce_path = "../data/scenarios/sce_" + sys + ".json";
    const string sim_path = "../data/simulations/" + sim + ".json";

    // System variables:
    CVData cvd; 
    MVData mvd;
    std::map<string, int> m_map;

    // Scenario variables:
    VectorXd z_min, z_max; /** Constraint vectors */
    MPCConfig conf; 
    MatrixXd du_tilde; 

    try { // Parse information:
        if (new_sim) {
            ParseNew(sce_path, m_map, cvd, mvd, conf, z_min, z_max);
        } else {
            // In order to change offset in u and y, this cvd, mvd and du_tilde need to be update here.
            Parse(sce_path, sim_path, m_map, cvd, mvd, conf, z_min, z_max, du_tilde); 
        }
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        exit(1);
    }

    // Determine simulation type:
    MPC_FSRM_Simulation sim_type;
    bool reduced_cost = (conf.W != 0); // Simulate smaller QP
    if (reduced_cost && conf.disable_slack) {
        sim_type = MPC_FSRM_Simulation::CONDENSED_W_WoSlack;
    } else if (conf.disable_slack) {
        sim_type = MPC_FSRM_Simulation::CONDENSED_WoSlack;
    } else if (reduced_cost) {
        sim_type = MPC_FSRM_Simulation::CONDENSED_W;
    } else {
        sim_type = MPC_FSRM_Simulation::CONDENSED;
    }

    switch (sim_type) {
        case MPC_FSRM_Simulation::CONDENSED: {
            FSRModel fsr(cvd.getSR(), m_map, conf, mvd.Inits, cvd.getInits());
            if (!new_sim) {
                fsr.setDuTildeMat(du_tilde); 
            }
            MatrixXd u_mat, y_pred;
            /** Optimized actuation, (n_MV, T) */ /** Predicted output (n_CV, T)*/

            try { // Solve
                MatrixXd ref = setRef(ref_vec, T, conf.P, m_map[kN_CV]);  /** Reference */
                SRSolver(T, u_mat, y_pred, fsr, conf, z_min, z_max, ref);
                if (new_sim) { // Serialize
                    SerializeSimulationNew(sim_path, sys, cvd, mvd, 
                    y_pred, u_mat, z_min, z_max, ref, fsr, T);
                } else {
                    SerializeSimulation(sim_path, y_pred, u_mat, ref, T);
                }
            }
            catch(std::exception& e) {
                std::cout << e.what() << std::endl;
                exit(1);
            }
            break;
        }

        case MPC_FSRM_Simulation::CONDENSED_W: {
            MPCConfig sim_conf = conf;
            sim_conf.W = 0;
            FSRModel fsr_sim(cvd.getSR(), m_map, sim_conf, mvd.Inits, cvd.getInits());
            FSRModel fsr_cost(cvd.getSR(), m_map, conf, mvd.Inits, cvd.getInits());

            if (!new_sim) {
                fsr_sim.setDuTildeMat(du_tilde); 
                fsr_cost.setDuTildeMat(du_tilde);
            }

            MatrixXd u_mat, y_pred;
            /** Optimized actuation, (n_MV, T) */ /** Predicted output (n_CV, T)*/ 

            try { // Solve
                MatrixXd ref = setRef(ref_vec, T, conf.P, m_map[kN_CV]); /** Reference */
                SRSolver(T, u_mat, y_pred, fsr_sim, fsr_cost, conf, z_min, z_max, ref);
                if (new_sim) { // Serialize
                    SerializeSimulationNew(sim_path, sys, cvd, mvd, 
                    y_pred, u_mat, z_min, z_max, ref, fsr_sim, T);
                } else {
                    SerializeSimulation(sim_path, y_pred, u_mat, ref, T);
                }
            }
            catch(std::exception& e) {
                std::cout << e.what() << std::endl;
                exit(1);
            }
            break;
        }

        case MPC_FSRM_Simulation::CONDENSED_WoSlack: {
            FSRModel fsr(cvd.getSR(), m_map, conf, mvd.Inits, cvd.getInits());
            if (!new_sim) {
                fsr.setDuTildeMat(du_tilde); 
            }
            MatrixXd u_mat, y_pred;
            /** Optimized actuation, (n_MV, T) */ /** Predicted output (n_CV, T)*/ 
           
            try { // Solve
                MatrixXd ref = setRef(ref_vec, T, conf.P, m_map[kN_CV]); /** Reference */
                SRSolverWoSlack(T, u_mat, y_pred, fsr, conf, z_min, z_max, ref);

                if (new_sim) { // Serialize
                    SerializeSimulationNew(sim_path, sys, cvd, mvd, 
                    y_pred, u_mat, z_min, z_max, ref, fsr, T);
                } else {
                    SerializeSimulation(sim_path, y_pred, u_mat, ref, T);
                }
            }
            catch(std::exception& e) {
                std::cout << e.what() << std::endl;
                exit(1);
            }
            break;
        }

        case MPC_FSRM_Simulation::CONDENSED_W_WoSlack: {
            MPCConfig sim_conf = conf;
            sim_conf.W = 0;
            FSRModel fsr_sim(cvd.getSR(), m_map, sim_conf, mvd.Inits, cvd.getInits());
            FSRModel fsr_cost(cvd.getSR(), m_map, conf, mvd.Inits, cvd.getInits());

            if (!new_sim) {
                fsr_sim.setDuTildeMat(du_tilde); 
                fsr_cost.setDuTildeMat(du_tilde);
            }

            MatrixXd u_mat, y_pred;
            /** Optimized actuation, (n_MV, T) */ /** Predicted output (n_CV, T)*/ 

            try { // Solve
                MatrixXd ref = setRef(ref_vec, T, conf.P, m_map[kN_CV]); /** Reference */
                SRSolverWoSlack(T, u_mat, y_pred, fsr_sim, fsr_cost, conf, z_min, z_max, ref);
                if (new_sim) { // Serialize
                    SerializeSimulationNew(sim_path, sys, cvd, mvd, 
                    y_pred, u_mat, z_min, z_max, ref, fsr_sim, T);
                } else {
                    SerializeSimulation(sim_path, y_pred, u_mat, ref, T);
                }
            }
            catch(std::exception& e) {
                std::cout << e.what() << std::endl;
                exit(1);
            }
            break;
        }
    }
}