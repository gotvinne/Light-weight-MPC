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

/**
 * @brief Write json object to file file according to filepath
 * 
 * @param data json object to be written
 * @param filepath string file 
 */
void WriteJson(const json& data, const std::string& filepath);

/**
 * @brief 
 * 
 * @param data 
 * @param filepath 
 * @param scenario 
 * @param T MPC horizon
 * @param n_CV Number of controlled variables
 * @param n_MV Number of manipulated variables 
 */
void FormatSimData(json& data, const std::string& filepath, const std::string& scenario, 
                        const int& T, const int& n_CV, const int& n_MV);

/**
 * @brief 
 * 
 * @param data 
 * @param cv_data CDData object
 * @param n_CV Number of controlled variables
 */
void FormatSimCV(json& data, const CVData& cv_data, int n_CV); //const Eigen::Vector& z_min, const Eigen::Vector& z_max);

/**
 * @brief 
 * 
 * @param data 
 * @param mv_data MVData object
 * @param n_MV Number of manipulated variables
 */
void FormatSimMV(json& data, const MVData& mv_data, int n_MV);

#endif  // FORMATTING_H
