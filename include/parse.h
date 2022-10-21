/**
 * @file parse.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

/** File parsing scenario and system files  */

#ifndef PARSE_H
#define PARSE_H

#include <nlohmann/json.hpp>
#include <Eigen/Dense>

#include <string>
#include <tuple>
#include <array>

using json = nlohmann::json;

// ------- System file specifiers ------- 
const int kModelParam = 3;

const std::string kModel = "model";
const std::string kN_CV = "n_CV";
const std::string kN_MV = "n_MV";

const std::string kN = "N";
const std::string kMV = "MV";
const std::string kCV = "CV";

const std::string kDescription = "description";
const std::string kId = "id";
const std::string kInit = "init";
const std::string kS = "S"; 
const std::string kR = "u";

// ------- Scenario file specifiers ------- 
const std::string kSystem = "system";
const std::string kT = "T";

const std::string kMPC = "MPC";
const std::string kP = "P";
const std::string kM = "M";
const std::string kW = "W";
const std::string kQ = "Q";
const std::string kR = "R";
const std::string kRo = "ro";

const std::string kC = "c"; 

// ------- ------- ------- ------- ------- 

json ReadJson(std::string filepath);

// System data
struct IOVariableData {
    std::string Description; 
    std::string Id;
    std::float Init;
    Eigen::MatrixXf S;
    Eigen::ArrayXf Ref;

    IOVariableInfo(std::string description, std::string id, float init, int n_MV, int n);
}

std::array<int,kModelParam> SystemModelData(json data);

// Scenario data
struct MPCConfig {
    int P;
    int M;
    int W; 

    Eigen::ArrayXf Q;
    Eigen::ArrayXf R; 
    float Ro; 

    MPCConfig(int p, int m, int w, float ro, int n_CV, int n_MV); 
}
void PrintContainer(std::array<int, 3> container);


#endif  // PARSE_H