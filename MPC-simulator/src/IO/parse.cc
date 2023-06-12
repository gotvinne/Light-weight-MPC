/**
 * @file parse.cc
 * @author Geir Ola Tvinnereim
 * @copyright Released under the terms of the BSD 3-Clause License
 * @date 2022
 */

#include "IO/parse.h"
#include "IO/json_specifiers.h"

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

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
 * @brief Checks if std::string is double or int
 * 
 * @param str string to be tested
 * @return true 
 * @return false 
 */
static bool IsDigit(string str) {
    for (auto &ch : str) {
        if (!isdigit(ch) && ch != '.') {
            return false;
        }
    }
    return true; 
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

static void ValidateConstraints(const VectorXd& z_min, const VectorXd& z_max, std::map<string, int>& m_map) {
    if (z_min.rows() != (2 * m_map[kN_MV] + m_map[kN_CV]) || z_min.rows() != (2 * m_map[kN_MV] + m_map[kN_CV])) {
        throw std::out_of_range("Constraints does not match system description, n_CV = " + std::to_string(m_map[kN_CV]) + " n_MV = " + std::to_string(m_map[kN_MV]));
    }

    for (int i = 0; i < z_min.size(); i++) {
        if (z_min[i] > z_max[i]) {
            throw std::invalid_argument("Lower constraint index, " + std::to_string(i) + ", takes a higher value then upper constraint");
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
        json cv_data = sys_data.at(kCV), mv_data = sys_data.at(kMV);
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
 * @param z_min Eigen::VectorXd lower constraint
 * @param z_max Eigen::VectorXd upper constraint 
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
 * @param mvd MVData
 */
static void ParseSimulationData(const json& sim_data, MatrixXd& du_tilde, MVData& mvd) {
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

        json mv_data = sim_data.at(kMV); // Only need information about du and u to simulate further
        int i = 0;
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
        std::cerr << "ERROR! Cannot read file: " << e.what() << std::endl; 
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

    // Error checking system and scenario parameters:
    if (conf.Q.rows() != m_map[kN_CV]) {
        throw std::out_of_range("Q matrix dimension does not match system description, n_CV = " + std::to_string(m_map[kN_CV]));
    }
    if (conf.R.rows() != m_map[kN_MV]) {
        throw std::out_of_range("R matrix dimension does not match system description, n_CV = " + std::to_string(m_map[kN_CV]));
    }
    if (conf.P > m_map[kN]) {
        throw std::out_of_range("Cannot predict further then P = N = " + std::to_string(m_map[kN]));
    }
    if (conf.M > conf.P) {
        throw std::out_of_range("Cannot predict further then M = P = " + std::to_string(conf.P));
    }
    if (conf.W > conf.M) {
        throw std::out_of_range("W must be smaller or equal then M =" + std::to_string(conf.M));
    }

    if (!conf.disable_slack) {
        if (conf.RoH.rows() != m_map[kN_CV]) {
            throw std::out_of_range("RoH dimension does not match system description, n_CV = " + std::to_string(m_map[kN_CV]));
        }
        if (conf.RoL.rows() != m_map[kN_CV]) {
            throw std::out_of_range("RoL dimension does not match system description, n_CV = " + std::to_string(m_map[kN_CV]));
        }
    }
    ValidateConstraints(z_min, z_max, m_map);
}

void Parse(const string& sce_filepath, const string& sim_filepath, std::map<string, int>& m_map,
            CVData& cvd, MVData& mvd, MPCConfig& conf, 
                VectorXd& z_min, VectorXd& z_max, MatrixXd& du_tilde) {
    
    ParseNew(sce_filepath, m_map, cvd, mvd, conf, z_min, z_max);

    // Parse simulation file:
    json sim_data = ReadJson(sim_filepath);
    ParseSimulationData(sim_data, du_tilde, mvd);
}

// Web application parse
void Parse(const string& sce_file, const string& sys_file, std::map<string, int>& m_map,
                    CVData& cvd, MVData& mvd, MPCConfig& conf, 
                    VectorXd& z_min, VectorXd& z_max) {
    json sce_data = json::parse(sce_file), sys_data = json::parse(sys_file); 

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

std::vector<double> ParseRefString(const string& ref_str) {
    // Remove whitespaces:
    string copy = ref_str;
    if (copy[0] != '[') {
        throw std::invalid_argument("Missing starting bracket in referece arg!");
    } else if (copy.back() != ']') {
        throw std::invalid_argument("Missing enclosing bracket in referece arg!");
    }

    string stripped = copy.substr(1, copy.size() - 2);
    const char sep = ',', space = ' ';
    std::replace(stripped.begin(), stripped.end(), sep, space);
 
    // Split string to std::vector
    std::vector<double> ref_vec; 
    std::stringstream ss(stripped);
    string item;
    while (ss >> item) {
        if (IsDigit(item)) {
            ref_vec.push_back(std::stod(item));
        }
    }
    return ref_vec;
}

