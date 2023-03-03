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

/**
 * @brief Reads a json file using nlohmann::json
 * 
 * @param filepath file to be read
 * @return json object
 */
nlohmann::json ReadJson(const std::string& filepath);

/**
 * @brief Combines all parse functionality, parsing system and scenario file
 * 
 * @param sce_filepath scenario filepath
 * @param model_param map of model parameters
 * @param cvd CV system data  
 * @param mvd MV system data
 * @param mpc_config MPC configuration data
 * @param z_min lower constraints 
 * @param z_max upper constraints
 */
void ParseNew(const std::string& sce_filepath, std::map<std::string, int>& model_param,
                    CVData& cvd, MVData& mvd, MPCConfig& mpc_config, 
                        Eigen::VectorXd& z_min, Eigen::VectorXd& z_max);

/**
 * @brief Parse function taking previous simulations into account
 * 
 * @param sce_filepath scenario filepath
 * @param sim_filepath simulation filepath
 * @param model_param map of model parameters
 * @param cvd CV system data  
 * @param mvd MV system data
 * @param mpc_config MPC configuration data
 * @param z_min lower constraints 
 * @param z_max upper constraints
 * @param du_tilde [Eigen::MatrixXd] Matrix of previous actuations
 */
void Parse(const std::string& sce_filepath, const std::string& sim_filepath, std::map<std::string, int>& model_param,
            CVData& cvd, MVData& mvd, MPCConfig& mpc_config, 
                Eigen::VectorXd& z_min, Eigen::VectorXd& z_max, Eigen::MatrixXd& du_tilde);

/**
 * @brief Parsing only the system in order to simulate open loop
 * 
 * @param system 
 * @param m_map 
 * @param cvd 
 * @param mvd 
 */
void ParseOpenLoop(const string& system, std::map<string, int>& m_map, CVData& cvd, MVData& mvd);
#endif // PARSE_H
