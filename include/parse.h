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
 */
void ParseSystemData(const json& sys_data, std::map<string, int>& model_param,
                    CVData& output_data, MVData& input_data, int T); 

/**
 * @brief High-level function parsing a scenario file
 * 
 * @param sce_data json object of scenario file
 * @param system corresponding system file
 * @param mpc_conf MPCConfig object
 * @param z_min Eigen::VectorXF 
 * @param z_max Eigen::VectorXf
 * @param n_CV number of control variables 
 * @param n_MV number of manipulated variables
 */
void ParseScenarioData(const json& sce_data, string& system, MPCConfig& mpc_conf, 
                        VectorXd& z_min, VectorXd& z_max, int n_CV, int n_MV);

/**
 * @brief 
 * 
 * @param sys_filepath 
 * @param sce_filepath 
 * @param model_param 
 * @param output_data 
 * @param input_data 
 * @param system 
 * @param mpc_config 
 * @param z_min 
 * @param z_max 
 * @param T 
 */
void Parse(const string& sys_filepath, const string& sce_filepath, std::map<string, int>& model_param,
                    CVData& output_data, MVData& input_data, string& system, MPCConfig& mpc_config, 
                        VectorXd& z_min, VectorXd& z_max, int T);

#endif  // PARSE_H
