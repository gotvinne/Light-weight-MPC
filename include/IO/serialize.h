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
 * @param filepath string file 
 */
void WriteJson(const json& data, const string& filepath);

/**
 * @brief 
 * 
 * @param data 
 * @param write_path 
 * @param scenario 
 * @param cv_data 
 * @param mv_data 
 * @param y_pred 
 * @param u_mat 
 * @param z_min
 * @param z_max
 * @param fsr
 * @param T 
 */
void SerializeSimulation(json& data, const string& write_path, const string& scenario, const CVData& cv_data, const MVData& mv_data, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const VectorXd& z_min, const VectorXd& z_max, const FSRModel& fsr, int T);    


#endif  // SERIALIZE_H
