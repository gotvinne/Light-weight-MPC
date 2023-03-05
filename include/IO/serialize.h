/**
 * @file serialize.h
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "IO/data_objects.h"
#include "model/FSRModel.h"

#include <string>

#include "Eigen/Dense"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using MatrixXd = Eigen::MatrixXd;
using VectorXd = Eigen::VectorXd;
using string = std::string; 

/**
 * @brief Write json object to file file according to filepath
 * 
 * @param data json object to be written
 * @param filepath [string] filepath to be written 
 */
void WriteJson(const json& data, const string& filepath);

/**
 * @brief Serialize simulation data into simulation JSON data files
 * 
 * @param write_path [string] file path to write json file
 * @param scenario [string] scenario name
 * @param cvd CVData
 * @param mvd MVData
 * @param y_pred [Eigen::MatrixXd]
 * @param u_mat [Eigen::MatrixXd]
 * @param z_min [Eigen::VectorXd] lower constraints
 * @param z_max [Eigen::VectorXd] upper constraints
 * @param fsr [FSRModel] 
 * @param T [int] MPC horizon
 */
void SerializeSimulationNew(const string& write_path, const string& scenario, const CVData& cvd, const MVData& mvd, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const VectorXd& z_min, const VectorXd& z_max, const FSRModel& fsr, int T);    

/**
 * @brief Serialize a simulation by appending data on previous simulations
 *        Simply done by reading the json file, updating it, and writing it again
 * 
 * @param write_path [string]
 * @param y_pred [M]
 * @param u_mat [Eigen::MatrixXd]
 * @param T [int] MPC horizon
 */
void SerializeSimulation(const string& write_path, const MatrixXd& y_pred, const MatrixXd& u_mat, int T);

void SerializeOpenLoop(const string& write_path, const string& scenario, const CVData& cvd, const MVData& mvd, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const FSRModel& fsr, int T);
                    
#endif  // SERIALIZE_H
