/**
 * @file formatting.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef FORMATTING_H
#define FORMATTING_H 

#include "IO/data_objects.h"

#include <string>

#include "Eigen/Dense"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using MatrixXd = Eigen::MatrixXd;
using string = std::string; 

/**
 * @brief Write json object to file file according to filepath
 * 
 * @param data json object to be written
 * @param filepath string file 
 */
void WriteJson(const json& data, const string& filepath);

/**
 * @brief Formats the plain data in simulation file
 * 
 * @param data 
 * @param system
 * @param scenario 
 * @param n_CV Number of controlled variables
 * @param n_MV Number of manipulated variables 
 * @param T MPC horizon
 */
void FormatSimData(json& data, const string& system, const string& scenario, 
                int n_CV, int n_MV, int T);

/**
 * @brief Formats the CV data in the simulation file
 * 
 * @param data 
 * @param cv_data CDData object
 * @param n_CV Number of controlled variables
 * @param z_min 
 * @param z_max
 */
void FormatSimCV(json& data, const CVData& cv_data, const MatrixXd& y_pred, int n_CV);

/**
 * @brief Formats the MV data in the simulation file
 * 
 * @param data 
 * @param mv_data MVData object
 * @param n_MV Number of manipulated variables
 */
void FormatSimMV(json& data, const MVData& mv_data, const MatrixXd& u, int n_MV);

/**
 * @brief 
 * 
 * @param data 
 * @param write_path 
 * @param system 
 * @param scenario 
 * @param cv_data 
 * @param mv_data 
 * @param y_pred 
 * @param u_mat 
 * @param n_CV 
 * @param n_MV 
 * @param T 
 */
void FormatScenario(json& data, const string& write_path, const string & system, const string& scenario, const CVData& cv_data, const MVData& mv_data, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, int n_CV, int n_MV, int T);

#endif  // FORMATTING_H
