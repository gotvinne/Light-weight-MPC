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
    std::ifstream file(filepath);
    return json::parse(file);
}

// Make this a pass by reference!
std::array<int, kModelParam> ModelData(const json& sys_data) {
    return std::array<int,kModelParam> {sys_data[kModel][kN_CV], sys_data[kModel][kN_MV],
                                        sys_data[kModel][kN]};
}

StateData::StateData(const json& sys_data, int n_MV, int N) {
    S = Eigen::MatrixXf::Zero(n_MV, N);
    // Load data in Matrix                                
}

InputData::InputData(const json& sys_data, int T) {
    Ref = Eigen::ArrayXf::Zero(T);
    // Load data in Matrix                                
}

MPCConfig::MPCConfig(const json& sce_data, int n_CV, int n_MV) {
    json mpc_data = sce_data.at(kMPC);
    P = mpc_data.at(kP);
    M = mpc_data.at(kM);
    W = mpc_data.at(kW);

    Eigen::ArrayXf q_arr = Eigen::ArrayXf::Zero(n_CV);
    Eigen::ArrayXf r_arr = Eigen::ArrayXf::Zero(n_MV);
    for (int i = 0; i < n_CV; i++) {
        q_arr[i] = mpc_data.at(kQ).at(i);
    }
    for (int i = 0; i < n_MV; i++) {
        r_arr[i] = mpc_data.at(kR).at(i);
    }

    Ro = mpc_data.at(kRo);
    bias_update = mpc_data.at(kBu);
}

Eigen::ArrayXf ConstraintData(const json& sce_data, Eigen::ArrayXf& arr, bool upper) {
    json j_arr(sce_data.at(kC));
    int size = j_arr.size();

    arr = Eigen::ArrayXf::Zero(size);
    for (int i = 0; i < size; i++) {
        for (auto& elem : j_arr.at(i).items()) {
            arr[i] = elem.value().at(upper);
        }
    }
    return arr;
}

void ParseScenarioData(const json& sce_data, std::string& system, MPCConfig& mpc_config, 
                        Eigen::ArrayXf upper_constraints, Eigen::ArrayXf lower_constraints,
                        int n_CV, int n_MV) {
    system = sce_data[kSystem];
    mpc_config = MPCConfig(sce_data, int n_CV, int n_MV);
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
