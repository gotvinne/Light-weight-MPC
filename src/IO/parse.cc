/**
 * @file parse.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "parse.h"
#include "json_specifiers.h"
#include "data_objects.h"

#include <nlohmann/json.hpp>
#include <Eigen/Dense>

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json; 

json ReadJson(const std::string& filepath) {
    try {
        std::ifstream file(filepath);
        return json::parse(file);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl; 
        return 1;
    }
}

void ModelData(const json& sys_data, std::map<std::string,int>& map) {
    try {
        json model_data = sys_data.at(kModel);
        map[kN_CV] = model_data.at(kN_CV);
        map[kN_MV] = model_data.at(kN_MV);
        map[kN] = model_data.at(kN);
    }
    catch(json::exception& e) {
        std::cout << e.what() << std::endl; 
    }    
}

void ConstraintData(const json& sce_data, VectorXd& arr, bool upper) {
    json j_arr(sce_data.at(kC));
    int size = j_arr.size();
    // Implement size check
    arr.resize(size);
    for (int i = 0; i < size; i++) {
        for (auto& elem : j_arr.at(i).items()) {
            arr[i] = elem.value().at(upper);
        }
    }
}

void ParseScenarioData(const json& sce_data, std::string& system, MPCConfig& mpc_config, 
                        VectorXd& z_max, VectorXd& z_min, int n_CV, int n_MV) {
    try {                     
        system = sce_data.at(kSystem);
        mpc_config = MPCConfig(sce_data, n_CV, n_MV);
        ConstraintData(sce_data, z_max, true); // NB! Size check
        ConstraintData(sce_data, z_min, false); //NB size check
    }
    catch(json::exception& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
    } 
}

void ParseSystemData(const json& sys_data, std::map<std::string, int>& model_param,
                    CVData& output_data, MVData& input_data, int T) {
    try {
        ModelData(sys_data, model_param);
        json cv_data = sys_data.at(kCV);
        json mv_data = sys_data.at(kMV);

        output_data = CVData(cv_data, model_param[kN_MV], model_param[kN_CV], model_param[kN], T);
        input_data = MVData(mv_data, model_param[kN_MV]); 
    }
    catch(json::exception& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
    } 
    catch(std::invalid_argument& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
    }
}

void PrintContainer(std::map<std::string,int> container) {
    for (auto ptr = container.begin(); ptr != container.end(); ptr++) {
        std::cout << ptr->first << ", " << ptr->second << std::endl;
    }
}
