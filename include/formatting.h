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

using json = nlohmann::json;

void WriteJson(const json& data, const std::string& filepath);

//void FormatSystemData(json& data, const std::string& filepath);
//void FormatScenarioData(json& data, const std::string& filepath); 
//void FormatSimulationData(json& data, const std::string&  filepath)

#endif  // FORMATTING_H