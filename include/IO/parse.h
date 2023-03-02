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
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

/**
 * @brief Reads a json file using nlohmann::json
 * 
 * @param filepath file to be read
 * @return json object
 */
json ReadJson(const string& filepath);

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
void ParseNew(const string& sce_filepath, std::map<string, int>& model_param,
                    CVData& cvd, MVData& mvd, MPCConfig& mpc_config, 
                        VectorXd& z_min, VectorXd& z_max);

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
void Parse(const string& sce_filepath, const string& sim_filepath, std::map<string, int>& model_param,
            CVData& cvd, MVData& mvd, MPCConfig& mpc_config, 
                VectorXd& z_min, VectorXd& z_max, MatrixXd& du_tilde);

#endif  // PARSE_H
