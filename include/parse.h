/**
 * @file parse.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

/** File parsing scenario and system files  */
#ifndef PARSE_H
#define PARSE_H

#include <nlohmann/json.hpp>
#include <Eigen/Dense>

#include <string>
#include <map>
#include <vector>

using json = nlohmann::json;

/**
 * @brief Reads a json file using nlohmann::json
 * 
 * @param filepath file to be read
 * @return json object
 */
json ReadJson(const std::string& filepath);

// System data
/**
 * @brief C++ struct representing state data described in system file
 * 
 */
struct CVData {
    std::vector<std::string> State; 
    std::vector<float> Init;
    Eigen::MatrixXf S;

    CVData();
    CVData(const json& cv_data, int n_MV, int n_CV, int N);
};

/**
 * @brief C++ struct representing input data 
 * 
 */
struct MVData {
    std::vector<std::string> Input; 
    std::vector<float> Init;
    Eigen::ArrayXf Ref;

    MVData();
    MVData(const json& mv_data, int n_MV, int T);
};

/**
 * @brief function obtaining model data from system file
 * 
 * @param sys_data json object of system file
 * @param map to be filled with model data
 */
void ModelData(const json& sys_data, std::map<std::string,int>& map);

/**
 * @brief high-level function parsing system file
 * 
 * @param sys_data json object of system file
 * @param model_param std::map holding model parameters
 * @param state_data CVData 
 * @param input_data MVData
 * @param T mpc horizon
 */
void ParseSystemData(const json& sys_data, std::map<std::string, int>& model_param,
                    CVData& state_data, MVData& input_data, int T); 

/**
 * @brief Assigns reference values to the Eigen::ArrayXf passed by reference
 * 
 * @param ref_data json object holding input reference data
 * @param ref Eigen::ArrayXf
 * @param start_index index to start to fill from
 * @param interval number of values to be filled
 */
void FillReference(const json& ref_data, Eigen::ArrayXf& ref, int start_index, int interval);

/**
 * @brief Assigns step coefficients to the Eigen::MatrixXf passed by reference 
 * 
 * @param s_data json object holding step coefficient data 
 * @param S Eigen::MatrixXf to be filled with step coefficients
 * @param n_MV number of manipulated variables
 * @param start_index index to start to fill from
 * @param interval number of values to be filled
 */
void FillStepCoMatrix(const json& s_data, Eigen::MatrixXf& S, int n_MV, int start_index, int interval);

// Scenario data
/**
 * @brief C++ struct representing the MPC spesifics described in the scenario file
 * 
 */
struct MPCConfig {
    int P;
    int M;
    int W; 

    Eigen::ArrayXf Q; // Container with dynamic size, not allocated yet
    Eigen::ArrayXf R; 
    float Ro; 
    bool bias_update;

    MPCConfig();
    MPCConfig(const json& sce_data, int n_CV, int n_MV); 
};

/**
 * @brief Fills an Eigen::ArrayXf with the corresponding constraint data from system file
 * 
 * @param sce_data json object of scenario file
 * @param arr Eigen::ArrayXf to hold the constraints [dU, U, Y]
 * @param upper bool indicating if upper constraints are returned, upper = false: lower constraints are returnd 
 */
void ConstraintData(const json& sce_data, Eigen::ArrayXf& arr, bool upper);

/**
 * @brief High-level function parsing a scenario file
 * 
 * @param sce_data json object of scenario file
 * @param system corresponding system file
 * @param mpc_conf MPCConfig object
 * @param upper_constraints Eigen::ArrayXf
 * @param lower_constraints Eigen::ArrayXF 
 * @param n_CV number of control variables 
 * @param n_MV number of manipulated variables
 */
void ParseScenarioData(const json& sce_data, std::string& system, MPCConfig& mpc_conf, 
                        Eigen::ArrayXf& upper_constraints, Eigen::ArrayXf& lower_constraints,
                        int n_CV, int n_MV);


void PrintContainer(std::map<std::string, int> container);

#endif  // PARSE_H
