/**
 * @file parse.cc
 * @author Geir Ola Tvinnereim
 * @copyright Released under the terms of the BSD 3-Clause License
 * @date 2022
 */

#include "IO/parse.h"
#include "IO/json_specifiers.h"
#include "IO/data_objects.h"

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

/**
 * @brief function obtaining model data from system file
 * 
 * @param sys_data json object of system file
 * @param map to be filled with model data
 */
static void ModelData(const json& sys_data, std::map<string,int>& map) {
    try {
        json model_data = sys_data.at(kModel);
        map[kN_CV] = model_data.at(kN_CV);
        map[kN_MV] = model_data.at(kN_MV);
        map[kN] = model_data.at(kN);
    }
    catch(json::exception& e) {
        std::cerr << e.what() << std::endl; 
    }    
}

/**
 * @brief Fills an Eigen::VectorXf with the corresponding constraint data from system file
 * 
 * @param sce_data json object of scenario file
 * @param arr Eigen::VectorXf to hold the constraints [dU, U, Y]
 * @param upper bool indicating if upper constraints are returned, upper = false: lower constraints are returnd 
 */
static void ConstraintData(const json& sce_data, VectorXd& arr, bool upper) {
    json j_arr = sce_data.at(kC);
    int size = j_arr.size();
    
    arr.resize(size);
    for (int i = 0; i < size; i++) {
        for (auto& elem : j_arr.at(i).items()) {
            arr(i) = elem.value().at(upper);
        }
    }
}

/**
 * @brief high-level function parsing system file
 * 
 * @param sys_data json object of system file
 * @param m_map std::map holding model parameters
 * @param output_data CVData 
 * @param input_data MVData
 */
static void ParseSystemData(const json& sys_data, std::map<string, int>& m_map,
                    CVData& output_data, MVData& input_data) {
    try {
        ModelData(sys_data, m_map);
        json cv_data = sys_data.at(kCV);
        json mv_data = sys_data.at(kMV);

        output_data = CVData(cv_data, m_map[kN_MV], m_map[kN_CV], m_map[kN]);
        input_data = MVData(mv_data, m_map[kN_MV]); 
    }
    catch(json::exception& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
    } 
    catch(std::invalid_argument& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
    }
}

/**
 * @brief High-level function parsing a scenario file
 * 
 * @param sce_data json object of scenario file
 * @param system corresponding system file
 * @param mpc_conf MPCConfig object
 * @param z_min Eigen::VectorXF 
 * @param z_max Eigen::VectorXf
 */
static void ParseScenarioData(const json& sce_data, string& system, MPCConfig& mpc_config, 
                        VectorXd& z_min, VectorXd& z_max) {
    try {                     
        system = sce_data.at(kSystem);
        mpc_config = MPCConfig(sce_data);
        ConstraintData(sce_data, z_max, true); 
        ConstraintData(sce_data, z_min, false);
    }
    catch(json::exception& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
    } 
}

/**
 * @brief Parsing simulation data for further simulations
 * 
 * @param sim_data [json] object holding json data
 * @param du_tilde [Eigen::MatrixXd] matrix holding previous actuations
 * @param cvd CVData
 * @param mvd MVData
 */
static void ParseSimulationData(const json& sim_data, MatrixXd& du_tilde, CVData& cvd, MVData& mvd) {
    try {               
        // Read du_tilde and yT, dT
        json du_tilde_data = sim_data.at(kDuTilde);
        int row = du_tilde_data.size(); // = n_MV
        int col = du_tilde_data.at(0).size(); // N-1 

        du_tilde.resize(row, col);
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                du_tilde(i, j) = du_tilde_data.at(i).at(j);
            }
        }

        json cv_data = sim_data.at(kCV);
        int i = 0;
        for (auto& cv : cv_data) {
            json y_arr = cv.at(kY_pred); // Should be changed to "y"
            cvd.setInits(y_arr.at(y_arr.size()-1), i);
            i++;
        }

        json mv_data = sim_data.at(kMV);
        i = 0;
        for (auto& mv : mv_data) {
            json u_arr = mv.at(kU);
            mvd.setInits(u_arr.at(u_arr.size()-1), i);
            i++;
        }
    }
    catch(json::exception& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
    } 
}

json ReadJson(const string& filepath) {
    try {
        std::ifstream file(filepath);
        return json::parse(file);
    }
    catch (std::exception& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
        return 1;
    }
}

void ParseNew(const string& sce_filepath, std::map<string, int>& m_map,
                    CVData& cvd, MVData& mvd, MPCConfig& conf, 
                        VectorXd& z_min, VectorXd& z_max) {
    // Parse scenario file
    json sce_data = ReadJson(sce_filepath);
    string system;
    ParseScenarioData(sce_data, system, conf, z_min, z_max);
    // Parse system file
    string sys_filepath = "../data/systems/" + system + ".json";
    json sys_data = ReadJson(sys_filepath);
    ParseSystemData(sys_data, m_map, cvd, mvd);

    if (conf.Q.rows() != m_map[kN_CV]) {
        throw std::out_of_range("Q matrix dimension does not match system description");
    }
    if (conf.R.rows() != m_map[kN_MV]) {
        throw std::out_of_range("R matrix dimension does not match system description");
    }
    if (conf.P > m_map[kN]) {
        throw std::out_of_range("Cannot predict further then P = N = " + std::to_string(m_map[kN]));
    }
    if (conf.M > m_map[kN]) {
        throw std::out_of_range("Cannot predict further then M = N = " + std::to_string(m_map[kN]));
    }
}

void Parse(const string& sce_filepath, const string& sim_filepath, std::map<string, int>& m_map,
            CVData& cvd, MVData& mvd, MPCConfig& conf, 
                VectorXd& z_min, VectorXd& z_max, MatrixXd& du_tilde) {
    
    ParseNew(sce_filepath, m_map, cvd, mvd, conf, z_min, z_max);

    // Parse simulation file:
    json sim_data = ReadJson(sim_filepath);
    ParseSimulationData(sim_data, du_tilde, cvd, mvd);
}

void Parse(const std::string& sce_file, const std::string& sys_file, std::map<std::string, int>& m_map,
                    CVData& cvd, MVData& mvd, MPCConfig& conf, 
                        Eigen::VectorXd& z_min, Eigen::VectorXd& z_max) {
    json sce_data = json::parse(sce_file); 
    json sys_data = json::parse(sys_file);

    string system; // Dummy variable
    ParseScenarioData(sce_data, system, conf, z_min, z_max);
    ParseSystemData(sys_data, m_map, cvd, mvd);
}

void ParseOpenLoop(const string& system, std::map<string, int>& m_map, CVData& cvd, MVData& mvd) {
    // Parse system file
    string sys_filepath = "../data/systems/" + system + ".json";
    json sys_data = ReadJson(sys_filepath);
    ParseSystemData(sys_data, m_map, cvd, mvd);
}

VectorXd* ParseReferenceStrByAllocation(string ref_str, int T, int P) {
    json ref_data = json::parse(ref_str);

    json ref_vec = ref_data.at(kRef);

    int size = int(ref_vec.size());
    VectorXd* y_ref = new VectorXd[size];

    for (int i = 0; i < size; i++) {
        y_ref[i] = VectorXd::Constant(T + P, ref_vec.at(i)); // Takes predictions into account!
    }
    return y_ref;
}
