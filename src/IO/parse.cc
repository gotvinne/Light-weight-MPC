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

json ReadJson(std::string filepath) {
    std::ifstream file(filepath);
    return json::parse(file);
}

std::array<int, kModelParam> ModelData(json sys_data) {
    return std::array<int,kModelParam> {sys_data[kModel][kN_CV], sys_data[kModel][kN_MV],
                                        sys_data[kModel][kN]};
}

StateData::StateData(json sys_data, int n_MV, int N) {
    S = Eigen::MatrixXf::Zero(n_MV, N);
    // Load data in Matrix                                
}

InputData::InputData(json sys_data, int T) {
    Ref = Eigen::ArrayXf::Zero(T);
    // Load data in Matrix                                
}

MPCConfig::MPCConfig(json sce_data, int n_CV, int n_MV) {
    // Load Q and R
}

Eigen::ArrayXf ConstraintData(json sce_data, bool upper) {
    json j_arr(sce_data.at(kC));
    int size = j_arr.size();

    Eigen::ArrayXf arr = Eigen::ArrayXf::Zero(size);
    for (int i = 0; i < size; i++) {
        for (auto& elem : j_arr.at(i).items()) {
            arr[i] = elem.value().at(upper);
        }
    }
    return arr;
}

void PrintContainer(std::array<int,3> container) {
    for (auto ptr = container.begin(); ptr != container.end(); ptr++) {
        std::cout << *ptr << std::endl;
    }
}
