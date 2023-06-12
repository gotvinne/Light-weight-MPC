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

/////////////////////////////////////////////////
///////// SERIALIZE SIMULATION FILES ////////////
/////////////////////////////////////////////////

/**
 * @brief Serialize simulation data into simulation JSON data files
 * 
 * @param write_path file path to write json file
 * @param scenario scenario name
 * @param cvd CVData
 * @param mvd MVData
 * @param y_pred matrix of CV predictions
 * @param u_mat MPC actuations
 * @param z_min lower constraints
 * @param z_max upper constraints
 * @param ref
 * @param fsr The finite step response model
 * @param T MPC horizon
 */
void SerializeSimulationNew(const string& write_path, const string& scenario, const CVData& cvd, const MVData& mvd, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const VectorXd& z_min, const VectorXd& z_max, 
                    const MatrixXd& ref, const FSRModel& fsr, int T);    

/**
 * @brief Serialize a simulation by appending data on previous simulations
 *        Simply done by reading the json file, updating it, and writing it again
 * 
 * @param write_path file path to write json file
 * @param y_pred matrix of CV predictions
 * @param u_mat MPC actuations
 * @param ref Reference data
 * @param T MPC horizon
 */
void SerializeSimulation(const string& write_path, const MatrixXd& y_pred, const MatrixXd& u_mat,
                        const MatrixXd& ref, int T);

/**
 * @brief Serialize Simulation returning a JSON string, used in Web application
 *
 * @param scenario scenario name
 * @param cvd CVData
 * @param mvd MVData
 * @param y_pred matrix of CV predictions
 * @param u_mat MPC actuations
 * @param z_min lower constraints
 * @param z_max upper constraints
 * @param ref reference data
 * @param fsr The finite step response model
 * @param T MPC horizon
 * @return string JSON file
 */
string SerializeSimulation(const string& scenario, const CVData& cvd, const MVData& mvd, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const VectorXd& z_min, const VectorXd& z_max,
                    const MatrixXd& ref, const FSRModel& fsr, int T);

/**
 * @brief Serializing an open loop simulation into simulation JSON data file
 * 
 * @param write_path file path to write json file
 * @param scenario scenario name
 * @param cvd CVData
 * @param mvd MVData
 * @param y_pred matrix of CV predictions
 * @param u_mat MPC actuations
 * @param fsr The finite step response model
 * @param T MPC horizon
 */
void SerializeOpenLoop(const string& write_path, const string& scenario, const CVData& cvd, const MVData& mvd, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const FSRModel& fsr, int T);

/////////////////////////////////////////////////
///////// SERIALIZE SCENARIO FILES //////////////
/////////////////////////////////////////////////

/**
 * @brief Serializing a scenario file
 * 
 * @param write_path file path to write json file
 * @param scenario scenario name
 * @param system system name
 * @param sys_path system filepath
 * @param mpc_m model parameters
 * @param Q output tuning 
 * @param R input tuning
 * @param Ro Slack variable tuning
 * @param l_du lower du constraint
 * @param l_u lower u constraint
 * @param l_y lower y constraint
 * @param u_du upper du constraint
 * @param u_u upper u constraint
 * @param u_y upper y constraint
 * @param n_CV number of controlled variables
 * @param n_MV number of manipulated variables
 */
void SerializeScenario(const string& write_path, const string& scenario, const string& system, const string& sys_path, std::map<string, int> mpc_m,
                     const VectorXd& Q, const VectorXd& R, const VectorXd& Ro, const VectorXd& l_du, 
                     const VectorXd& l_u, const VectorXd& l_y, const VectorXd& u_du, const VectorXd& u_u, const VectorXd& u_y,
                     int n_CV, int n_MV);

#endif // SERIALIZE_H
