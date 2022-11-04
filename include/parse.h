/**
 * @file parse.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

/** File parsing scenario and system files  */
#ifndef PARSE_H
#define PARSE_H

#include "data_objects.h"

#include <nlohmann/json.hpp>
#include <Eigen/Dense>

#include <string>
#include <map>

using json = nlohmann::json;

/**
 * @brief Reads a json file using nlohmann::json
 * 
 * @param filepath file to be read
 * @return json object
 */
json ReadJson(const std::string& filepath);

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
 * @param output_data CVData 
 * @param input_data MVData
 * @param T mpc horizon
 */
void ParseSystemData(const json& sys_data, std::map<std::string, int>& model_param,
                    CVData& output_data, MVData& input_data, int T); 

/**
 * @brief Fills an Eigen::VectorXf with the corresponding constraint data from system file
 * 
 * @param sce_data json object of scenario file
 * @param arr Eigen::VectorXf to hold the constraints [dU, U, Y]
 * @param upper bool indicating if upper constraints are returned, upper = false: lower constraints are returnd 
 */
void ConstraintData(const json& sce_data, Eigen::VectorXf& arr, bool upper);

/**
 * @brief High-level function parsing a scenario file
 * 
 * @param sce_data json object of scenario file
 * @param system corresponding system file
 * @param mpc_conf MPCConfig object
 * @param z_max Eigen::VectorXf
 * @param z_min Eigen::VectorXF 
 * @param n_CV number of control variables 
 * @param n_MV number of manipulated variables
 */
void ParseScenarioData(const json& sce_data, std::string& system, MPCConfig& mpc_conf, 
                        Eigen::VectorXf& z_max, Eigen::VectorXf& z_min, int n_CV, int n_MV);


void PrintContainer(std::map<std::string, int> container);

#endif  // PARSE_H
