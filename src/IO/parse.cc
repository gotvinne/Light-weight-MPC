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

json read_json(std::string filepath) {
    std::ifstream file (filepath);
    return json::parse(file);
}

std::array<int, kModelParam> system_model_data(json data) {
    return std::array<int,kModelParam> {data[kModel][kN_CV], data[kModel][kN_MV]
                                        , data[kModel][kN]};
}

void print_container(std::array<int,3> container) {
    for (auto ptr = container.begin(); ptr != container.end(); ptr++) {
        std::cout << *ptr << std::endl;
    }
}

IOVariableInfo::IOVariableInfo(std::string description, std::string id, float init,
                                int n_MV, int N) : description{description}, id{id}, init{init} {
    S = Eigen::MatrixXd::Zero(n_NM, N);
    // Load data in Matrix                                
}








