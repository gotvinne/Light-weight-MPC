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
    std::ifstream file (filepath);
    return json::parse(file);
}

std::array<int, kModelParam> SystemModelData(json data) {
    return std::array<int,kModelParam> {data[kModel][kN_CV], data[kModel][kN_MV]
                                        , data[kModel][kN]};
}

void PrintContainer(std::array<int,3> container) {
    for (auto ptr = container.begin(); ptr != container.end(); ptr++) {
        std::cout << *ptr << std::endl;
    }
}

StateData::StateData(json data, int n_MV, int N) {
    S = Eigen::MatrixXf::Zero(n_MV, N);
    // Load data in Matrix                                
}

InputData::InputData(json data, int T) {
    Ref = Eigen::ArrayXf::Zero(T);
    // Load data in Matrix                                
}

MPCConfig::MPCConfig(json data, int n_CV, int n_MV) {
    // Load Q and R
}

Eigen::ArrayXf ScenarioUpperConstraintData(json data) {
    
}

Eigen::ArrayXf ScenarioLowerConstraintData(json data) {
    
}

//Eigen::ArrayXf ScenarioLowerConstraintData(json data) {

//}








