/**
 * @file formatting.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef FORMATTING_H
#define FORMATTING_H 

#include "data_objects.h"

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
 * @param filepath 
 * @param scenario 
 * @param T MPC horizon
 * @param n_CV Number of controlled variables
 * @param n_MV Number of manipulated variables 
 */
void FormatSimData(json& data, const string& filepath, const string& scenario, 
                    int T, int n_CV, int n_MV);

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
void FormatSimMV(json& data, const MVData& mv_data, int n_MV);

void FillVector(json& vector, const MatrixXd& mat, int row);

#endif  // FORMATTING_H
