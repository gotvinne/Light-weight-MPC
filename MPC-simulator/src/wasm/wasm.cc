/**
 * @file wasm.cc
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "wasm/wasm.h"
#include "simulations.h"
#include "IO/data_objects.h"
#include "IO/json_specifiers.h"
#include "IO/parse.h"
#include "IO/serialize.h"

#include "MPC/solvers.h"
#include "model/FSRModel.h"

#include <map>
#include <vector>
#include <Eigen/Dense>
#include <nlohmann/json.hpp>

using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using json = nlohmann::json; 

/**
 * @brief Parse JSON reference to Eigen::MatrixXd, used in Web application
 * 
 * @param ref_str JSON formatted string
 * @param T MPC horison 
 * @param P Prediction horizon
 * @return Allocated Eigen::MatrixXd holding references
 */
static MatrixXd ParseReferenceStr(string ref_str, int T, int P) {
    // ref_str { 
    //      "ref": [ref1, ref2, ..., refn_CV]    
    // }
    json ref_data = json::parse(ref_str);
    json ref_vec = ref_data.at(kRef);

    int size = int(ref_vec.size());
    MatrixXd ref = MatrixXd::Zero(size, T + P);

    for (int i = 0; i < size; i++) {
        ref.row(i) = VectorXd::Constant(T + P, ref_vec.at(i)); // Takes predictions into account!
    }
    return ref;
}

string simulate(string sce_file, string sys_file, string sce, string ref_str, int T) {
    // System variables:
    CVData cvd; 
    MVData mvd;
    std::map<string, int> m_map;

    // Scenario variables:
    VectorXd z_min, z_max; /** Constraint vectors */
    MPCConfig conf; /** MPC configuration */

    // Parse information:
    try {
        Parse(sce_file, sys_file, m_map, cvd, mvd, conf, z_min, z_max);
    }
    catch(std::exception& e) {
        return string(e.what());
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

    string sim_results;
    switch (sim_type) {
        case MPC_FSRM_Simulation::CONDENSED: {
            FSRModel fsr(cvd.getSR(), m_map, conf, mvd.Inits, cvd.getInits());
            
            // MPC variables:
            MatrixXd u_mat, y_pred, ref = ParseReferenceStr(ref_str, T, conf.P);
            try { // Solve
                SRSolver(T, u_mat, y_pred, fsr, conf, z_min, z_max, ref);
            }
            catch(std::exception& e) {
                sim_results = string(e.what());
                break;
            }
            sim_results = SerializeSimulation(sce, cvd, mvd, 
              y_pred, u_mat, z_min, z_max, ref, fsr, T);
            break;
        }

        case MPC_FSRM_Simulation::CONDENSED_W: {
            MPCConfig sim_conf = conf;
            sim_conf.W = 0;
            FSRModel fsr_sim(cvd.getSR(), m_map, sim_conf, mvd.Inits, cvd.getInits());
            FSRModel fsr_cost(cvd.getSR(), m_map, conf, mvd.Inits, cvd.getInits());

            // MPC variables:
            MatrixXd u_mat, y_pred, ref = ParseReferenceStr(ref_str, T, conf.P);

            try { // Solve
                SRSolver(T, u_mat, y_pred, fsr_sim, fsr_cost, conf, z_min, z_max, ref);
            }
            catch(std::exception& e) {
                sim_results = string(e.what());
                break;
            }
            sim_results = SerializeSimulation(sce, cvd, mvd, 
              y_pred, u_mat, z_min, z_max, ref, fsr_sim, T);
            break;
        }

        case MPC_FSRM_Simulation::CONDENSED_WoSlack: {
            FSRModel fsr(cvd.getSR(), m_map, conf, mvd.Inits, cvd.getInits());
            
            // MPC variables:
            MatrixXd u_mat, y_pred, ref = ParseReferenceStr(ref_str, T, conf.P);
            try { // Solve
                SRSolverWoSlack(T, u_mat, y_pred, fsr, conf, z_min, z_max, ref);
            }
            catch(std::exception& e) {
                sim_results = string(e.what());
                break;
            }
            sim_results = SerializeSimulation(sce, cvd, mvd, 
              y_pred, u_mat, z_min, z_max, ref, fsr, T);
            break;
        }

        case MPC_FSRM_Simulation::CONDENSED_W_WoSlack: {
            MPCConfig sim_conf = conf;
            sim_conf.W = 0;
            FSRModel fsr_sim(cvd.getSR(), m_map, sim_conf, mvd.Inits, cvd.getInits());
            FSRModel fsr_cost(cvd.getSR(), m_map, conf, mvd.Inits, cvd.getInits());

            // MPC variables:
            MatrixXd u_mat, y_pred, ref = ParseReferenceStr(ref_str, T, conf.P);
            try { // Solve
                SRSolverWoSlack(T, u_mat, y_pred, fsr_sim, fsr_cost, conf, z_min, z_max, ref);
            }
            catch(std::exception& e) {
                sim_results = string(e.what());
                break;
            }
            sim_results = SerializeSimulation(sce, cvd, mvd, 
              y_pred, u_mat, z_min, z_max, ref, fsr_sim, T);
            break;
        }
    }
    return sim_results;
}