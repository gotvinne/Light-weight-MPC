/**
 * @file formatting
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void WriteJson(const json& data, std::string filepath) {
    std::ofstream ofs(filepath);
    ofs << data << std::endl;
}

void FormatSimulationData(json& data, std::string filepath) {

}