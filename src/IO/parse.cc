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

IOVariableData::IOVariableData(std::string description, std::string id, float init,
                                int n_MV, int N) : Description{description}, id{id}, init{init} {
    S = Eigen::MatrixXd::Zero(n_NM, N);
    // Load data in Matrix                                
}

MPCConfig::MPCConfig(int P, int M, int W, float ro, int n_CV, int n_MV) : P{p}, M{m}, W{w}, Ro{ro} {
    // Load Q and R
}








