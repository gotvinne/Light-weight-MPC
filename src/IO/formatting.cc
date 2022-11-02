/**
 * @file formatting
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "formatting.h"

#include <iostream>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json; 

void WriteJson(const json& data, const std::string& filepath) {
    std::ofstream ofs(filepath);
    ofs << data << std::endl; // Read in data
}

//void FormatSimulationData(json& data, std::string filepath) {

//}