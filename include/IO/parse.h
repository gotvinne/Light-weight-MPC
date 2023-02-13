/**
 * @file parse.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

/** File parsing scenario and system files  */
#ifndef PARSE_H
#define PARSE_H

#include "IO/data_objects.h"

#include <string>
#include <map>

#include <Eigen/Dense>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using VectorXd = Eigen::VectorXd;
using string = std::string;

/**
 * @brief Reads a json file using nlohmann::json
 * 
 * @param filepath file to be read
 * @return json object
 */
json ReadJson(const string& filepath);

/**
 * @brief high-level function parsing system file
 * 
 * @param sys_data json object of system file
 * @param model_param std::map holding model parameters
 * @param output_data CVData 
 * @param input_data MVData
 * @param T mpc horizon
 * @param P Prediction horizon
 */
void ParseSystemData(const json& sys_data, std::map<string, int>& model_param,
                    CVData& output_data, MVData& input_data, int T, int P); 

/**
 * @brief High-level function parsing a scenario file
 * 
 * @param sce_data json object of scenario file
 * @param system corresponding system file
 * @param mpc_conf MPCConfig object
 * @param z_min Eigen::VectorXF 
 * @param z_max Eigen::VectorXf
 */
void ParseScenarioData(const json& sce_data, string& system, MPCConfig& mpc_conf, 
                        VectorXd& z_min, VectorXd& z_max);

/**
 * @brief Combines all parse functionality, parsing system and scenario file
 * 
 * @param sce_filepath scenario filepath
 * @param model_param map of model parameters
 * @param output_data CV system data  
 * @param input_data MV system data
 * @param mpc_config MPC configuration data
 * @param z_min lower constraints 
 * @param z_max upper constraints
 * @param T MPC horizon
 */
void Parse(const string& sce_filepath, std::map<string, int>& model_param,
                    CVData& output_data, MVData& input_data, MPCConfig& mpc_config, 
                        VectorXd& z_min, VectorXd& z_max, int T);

#endif  // PARSE_H
