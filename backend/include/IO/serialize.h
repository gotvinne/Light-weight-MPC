/**
 * @file serialize.h
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright  Released under the terms of the BSD 3-Clause License
 * 
 */

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "IO/data_objects.h"
#include "model/FSRModel.h"

#include <string>
#include <map>

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
 * @param y_pred [Eigen::MatrixXd] matrix of CV predictions
 * @param u_mat [Eigen::MatrixXd] actuations
 * @param z_min [Eigen::VectorXd] lower constraints
 * @param z_max [Eigen::VectorXd] upper constraints
 * @param fsr [FSRModel] The finite step response model
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

/**
 * @brief Serialize Simulation returning a JSON string
 *
 * @param scenario [string] scenario name
 * @param cvd CVData
 * @param mvd MVData
 * @param y_pred [Eigen::MatrixXd] matrix of CV predictions
 * @param u_mat [Eigen::MatrixXd] actuations
 * @param z_min [Eigen::VectorXd] lower constraints
 * @param z_max [Eigen::VectorXd] upper constraints
 * @param fsr [FSRModel] The finite step response model
 * @param T [int] MPC horizon
 * @return string 
 */
string SerializeSimulation(const string& scenario, const CVData& cvd, const MVData& mvd, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const VectorXd& z_min, const VectorXd& z_max, const FSRModel& fsr, int T);

/**
 * @brief Serializing an open loop simulation into simulation JSON data file
 * 
 * @param write_path [string] file path to write json file
 * @param scenario [string] scenario name
 * @param cvd CVData
 * @param mvd MVData
 * @param y_pred [Eigen::MatrixXd]
 * @param u_mat actuations
 * @param fsr [FSRModel] 
 * @param T [int] MPC horizon
 */
void SerializeOpenLoop(const string& write_path, const string& scenario, const CVData& cvd, const MVData& mvd, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const FSRModel& fsr, int T);

/**
 * @brief Serializing a scenario file
 * 
 * @param write_path 
 * @param scenario 
 * @param system 
 * @param sys_path
 * @param mpc_m 
 * @param Q 
 * @param R 
 * @param Ro 
 * @param bias_update 
 * @param l_du 
 * @param l_u 
 * @param l_y 
 * @param u_du 
 * @param u_u 
 * @param u_y 
 * @param n_CV 
 * @param n_MV 
 */
void SerializeScenario(const string& write_path, const string& scenario, const string& system, const string& sys_path, std::map<string, int> mpc_m,
                     const VectorXd& Q, const VectorXd& R, const VectorXd& Ro, bool bias_update, const VectorXd& l_du, 
                     const VectorXd& l_u, const VectorXd& l_y, const VectorXd& u_du, const VectorXd& u_u, const VectorXd& u_y,
                     int n_CV, int n_MV);

#endif  // SERIALIZE_H
