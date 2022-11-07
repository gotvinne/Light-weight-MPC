/**
 * @file formatting
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "formatting.h"
#include "json_specifiers.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json; 

void WriteJson(const json& data, const std::string& filepath) {
    std::ofstream ofs(filepath);
    ofs << data.dump(4) << std::endl; // Read in data
    ofs.close();
}

void FormatSimData(json& data, const std::string& filepath, const std::string& scenario,
                        const int& T, const int& n_CV, const int& n_MV) {
    data[kScenario] = scenario;
    data[kT] = T;
    data[kN_CV] = n_CV;
    data[kN_MV] = n_MV; 
}

void FormatSimCV(json& data, const CVData& cv_data, int n_CV) {
    json arr = json::array(); 
    
    std::vector<std::string> outputs = cv_data.getOutputs();
    std::vector<std::string> units = cv_data.getUnits();

    for (int i = 0; i < n_CV; i++) {
        json obj = json::object();
        obj[kOutput] = outputs[i];
        obj[kUnit] = units[i];
        // Fill inn c, y, y_hat
        arr.push_back(obj);
    }
    data[kCV] = arr;
}