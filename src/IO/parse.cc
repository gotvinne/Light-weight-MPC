/**
 * @file parse.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "parse.h"

#include <nlohmann/json.hpp>
#include <Eigen/Dense>

#include <string>
#include <iostream>
#include <fstream>

json ReadJson(const std::string& filepath) {
    try {
        std::ifstream file(filepath);
        return json::parse(file);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl; 
    }
}

void ModelData(const json& sys_data, std::array<int, kModelParam>& arr) {
    arr = {sys_data.at(kModel).at(kN_CV), sys_data.at(kModel).at(kN_MV),
            sys_data.at(kModel).at(kN)};
}

StateData::StateData(const json& sys_data, int n_MV, int N) {
    S = Eigen::MatrixXf::Zero(n_MV, N);
    // Load data in Matrix                                
}

InputData::InputData(const json& sys_data, int T) {
    Ref = Eigen::ArrayXf::Zero(T);
    // Load data in Matrix                                
}

MPCConfig::MPCConfig() : P(), M(), W(), Ro(), bias_update() {}

/* Errorhandling, 
    Always use at() for type checking. 

*/
MPCConfig::MPCConfig(const json& sce_data, int n_CV, int n_MV) {
    try {
        json mpc_data = sce_data.at(kMPC);
        P = mpc_data.at(kP);
        M = mpc_data.at(kM);
        W = mpc_data.at(kW);

        Q.resize(n_CV); 
        R.resize(n_MV);
        for (int i = 0; i < n_CV; i++) {
            Q[i] = mpc_data.at(kQ).at(i);
        }
        for (int i = 0; i < n_MV; i++) {
            R[i] = mpc_data.at(kR).at(i);
        }

        Ro = mpc_data.at(kRo);
        bias_update = mpc_data.at(kBu);
    }
    catch(json::exception& e) {
        std::cout << e.what() << std::endl; 
    } 
}

Eigen::ArrayXf ConstraintData(const json& sce_data, Eigen::ArrayXf& arr, bool upper) {
    try {
        json j_arr(sce_data.at(kC));
        int size = j_arr.size();

        arr.resize(size);
        for (int i = 0; i < size; i++) {
            for (auto& elem : j_arr.at(i).items()) {
                arr[i] = elem.value().at(upper);
            }
        }
        return arr;
    }
    catch(json::exception& e) {
        std::cout << e.what() << std::endl; 
    } 
}

void ParseScenarioData(const json& sce_data, std::string& system, MPCConfig& mpc_config, 
                        Eigen::ArrayXf upper_constraints, Eigen::ArrayXf lower_constraints,
                        int n_CV, int n_MV) {
    system = sce_data.at(kSystem);
    mpc_config = MPCConfig(sce_data, n_CV, n_MV);
    ConstraintData(sce_data, upper_constraints, true);
    ConstraintData(sce_data, lower_constraints, false);
}

void ParseSystemData(const json& sys_data, std::array<int, kModelParam>& model_param,
                    StateData& state_data, InputData& input_data) {
    
}

void PrintContainer(std::array<int,3> container) {
    for (auto ptr = container.begin(); ptr != container.end(); ptr++) {
        std::cout << *ptr << std::endl;
    }
}
