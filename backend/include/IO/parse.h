/**
 * @file parse.h
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
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
 * @param m_map map of model parameters
 * @param cvd CV system data  
 * @param mvd MV system data
 * @param conf MPC configuration data
 * @param z_min lower constraints 
 * @param z_max upper constraints
 */
void ParseNew(const string& sce_filepath, std::map<string, int>& m_map,
                    CVData& cvd, MVData& mvd, MPCConfig& conf, 
                        VectorXd& z_min, VectorXd& z_max);

/**
 * @brief 
 * 
 * @param sce_file 
 * @param sys_file 
 * @param m_map 
 * @param cvd 
 * @param mvd 
 * @param conf 
 * @param z_min 
 * @param z_max 
 */
void Parse(const string& sce_file, const string& sys_file, std::map<string, int>& m_map,
                    CVData& cvd, MVData& mvd, MPCConfig& conf, 
                        VectorXd& z_min, VectorXd& z_max);
        
/**
 * @brief Parse function taking previous simulations into account
 * 
 * @param sce_filepath scenario filepath
 * @param sim_filepath simulation filepath
 * @param m_map of model parameters
 * @param cvd CV system data  
 * @param mvd MV system data
 * @param conf MPC configuration data
 * @param z_min lower constraints 
 * @param z_max upper constraints
 * @param du_tilde [Eigen::MatrixXd] Matrix of previous actuations
 */
void Parse(const string& sce_filepath, const string& sim_filepath, std::map<string, int>& m_map,
            CVData& cvd, MVData& mvd, MPCConfig& conf, 
                VectorXd& z_min, VectorXd& z_max, MatrixXd& du_tilde);

/**
 * @brief Parsing only the system in order to simulate open loop
 * 
 * @param system 
 * @param m_map 
 * @param cvd 
 * @param mvd 
 */
void ParseOpenLoop(const string& system, std::map<string, int>& m_map, CVData& cvd, MVData& mvd);

/**
 * @brief Parse JSON reference to Eigen::VectorXd, used in Web application
 * 
 * @param ref_str JSON formatted string
 * @param T MPC horison 
 * @param P Prediction horizon
 * @return Allocated Eigen::VectorXd holding references
 */
VectorXd* ParseReferenceStrByAllocation(string ref_str, int T, int P);

/**
 * @brief Another implementation with size check
 * 
 * @param ref_str 
 * @param T 
 * @param P 
 * @param n_CV 
 * @return VectorXd* 
 */
VectorXd* ParseReferenceStrByAllocation(const string& ref_str, int T, int P, int n_CV);
#endif // PARSE_H
