/**
 * @file data_structs.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <vector>
#include <string>

#include <nlohmann/json.hpp>
#include <Eigen/Dense>

using json = nlohmann::json; 

/**
 * @brief C++ struct representing state data described in system file
 * 
 */
struct CVData {
    std::vector<std::string> Outputs; 
    std::vector<float> Inits;
    std::vector<std::string> Units;
    Eigen::MatrixXf S;
    Eigen::VectorXf Y_Ref;

    CVData();
    CVData(const json& cv_data, int n_MV, int n_CV, int N, int T);
};

/**
 * @brief C++ struct representing input data 
 * 
 */
struct MVData {
    std::vector<std::string> Inputs; 
    std::vector<float> Inits;
    std::vector<std::string> Units;

    MVData();
    MVData(const json& mv_data, int n_MV);
};

/**
 * @brief C++ struct representing the MPC spesifics described in the scenario file
 * 
 */
struct MPCConfig {
    int P;
    int M;
    int W; 

    Eigen::VectorXf Q; // Container with dynamic size, not allocated yet
    Eigen::VectorXf R; 
    float Ro; 
    bool bias_update;

    MPCConfig();
    MPCConfig(const json& sce_data, int n_CV, int n_MV); 
};

/**
 * @brief Assigns reference values to the Eigen::VectorXf passed by reference
 * 
 * @param ref_data json object holding input reference data
 * @param ref Eigen::VectorXf
 * @param start_index index to start to fill from
 * @param interval number of values to be filled
 */
void FillReference(const json& ref_data, Eigen::VectorXf& ref, int start_index, int interval);

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


#endif // DATA_STRUCTS_H
