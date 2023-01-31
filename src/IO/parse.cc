/**
 * @file parse.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "IO/parse.h"
#include "IO/json_specifiers.h"
#include "IO/data_objects.h"

#include <nlohmann/json.hpp>
#include <Eigen/Dense>

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json; 
using VectorXd = Eigen::VectorXd;
using string = std::string;

/**
 * @brief function obtaining model data from system file
 * 
 * @param sys_data json object of system file
 * @param map to be filled with model data
 */
static void ModelData(const json& sys_data, std::map<string,int>& map) {
    try {
        json model_data = sys_data.at(kModel);
        map[kN_CV] = model_data.at(kN_CV);
        map[kN_MV] = model_data.at(kN_MV);
        map[kN] = model_data.at(kN);
    }
    catch(json::exception& e) {
        std::cerr << e.what() << std::endl; 
    }    
}

/**
 * @brief Fills an Eigen::VectorXf with the corresponding constraint data from system file
 * 
 * @param sce_data json object of scenario file
 * @param arr Eigen::VectorXf to hold the constraints [dU, U, Y]
 * @param upper bool indicating if upper constraints are returned, upper = false: lower constraints are returnd 
 */
static void ConstraintData(const json& sce_data, VectorXd& arr, bool upper) {
    json j_arr(sce_data.at(kC));
    int size = j_arr.size();
    
    arr.resize(size);
    for (int i = 0; i < size; i++) {
        for (auto& elem : j_arr.at(i).items()) {
            arr[i] = elem.value().at(upper);
        }
    }
}

json ReadJson(const string& filepath) {
    try {
        std::ifstream file(filepath);
        return json::parse(file);
    }
    catch (std::exception& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
        return 1;
    }
}

void ParseSystemData(const json& sys_data, std::map<string, int>& model_param,
                    CVData& output_data, MVData& input_data, int T) {
    try {
        ModelData(sys_data, model_param);
        json cv_data = sys_data.at(kCV);
        json mv_data = sys_data.at(kMV);

        output_data = CVData(cv_data, model_param[kN_MV], model_param[kN_CV], model_param[kN], T);
        
        VectorXd vec = output_data.getYRef()[0];
        for (int i = 0; i < vec.rows(); i++) {
            std::cout << vec(i) << std::endl;
        }
        
        //input_data = MVData(mv_data, model_param[kN_MV]); 
    }
    catch(json::exception& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
    } 
    catch(std::invalid_argument& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
    }
}

void ParseScenarioData(const json& sce_data, string& system, MPCConfig& mpc_config, 
                        VectorXd& z_min, VectorXd& z_max) {
    try {                     
        system = sce_data.at(kSystem);
        mpc_config = MPCConfig(sce_data);
        ConstraintData(sce_data, z_max, true); 
        ConstraintData(sce_data, z_min, false);
    }
    catch(json::exception& e) {
        std::cerr << "ERROR! " << e.what() << std::endl; 
    } 
}

void Parse(const string& sce_filepath, std::map<string, int>& model_param,
                    CVData& output_data, MVData& input_data, MPCConfig& mpc_config, 
                        VectorXd& z_min, VectorXd& z_max, int T) {
    // Parse scenario
    json sce_data = ReadJson(sce_filepath);
    string system;
    ParseScenarioData(sce_data, system, mpc_config, z_min, z_max);
   
    // Parse system
    string sys_filepath = "../data/systems/" + system + ".json";
    json sys_data = ReadJson(sys_filepath);
    ParseSystemData(sys_data, model_param, output_data, input_data, T);
}
