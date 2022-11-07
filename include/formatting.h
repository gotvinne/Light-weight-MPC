/**
 * @file formatting.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef FORMATTING_H
#define FORMATTING_H 

#include <nlohmann/json.hpp>
#include <string>
#include "data_objects.h"

#include "Eigen/Dense"

using json = nlohmann::json;

void WriteJson(const json& data, const std::string& filepath);

void FormatSimData(json& data, const std::string& filepath, const std::string& scenario, 
                        const int& T, const int& n_CV, const int& n_MV);
void FormatSimCV(json& data, const CVData& cv_data, int n_CV); //const Eigen::Vector& z_min, const Eigen::Vector& z_max);

void FormatSimMV(json& data, const MVData& mv_data, int n_MV);
#endif  // FORMATTING_H