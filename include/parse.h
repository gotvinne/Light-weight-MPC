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

#include <array>

using json = nlohmann::json;

// ------- System file specifiers ------- 
const int kModelParam = 3;

const std::string kModel = "model";
const std::string kN_CV = "n_CV";
const std::string kN_MV = "n_MV";
const std::string kN = "N";

const std::string kState = "state";
const std::string kInput = "input";
const std::string kInit = "init";
const std::string kS = "S"; 
const std::string kU = "u";

// ------- Scenario file specifiers ------- 
const std::string kSystem = "system";

const std::string kMPC = "MPC";
const std::string kP = "P";
const std::string kM = "M";
const std::string kW = "W";
const std::string kQ = "Q";
const std::string kR = "R";
const std::string kRo = "Ro";
const std::string kBu = "bias update";

const std::string kC = "c_i"; 

// ------- ------- ------- ------- ------- 

json ReadJson(const std::string& filepath);

// System data
struct StateData {
    std::vector<std::string> State; 
    std::vector<float> Init;
    Eigen::MatrixXf S;

    StateData();
    StateData(const json& cv_data, int n_MV, int N);
};

struct InputData {
    std::vector<std::string> Input; 
    std::vector<float> Init;
    Eigen::ArrayXf Ref;

    InputData();
    InputData(const json& mv_data, int T);
};

void ModelData(const json& sys_data, std::array<int,kModelParam>& arr);
void ParseSystemData(const json& sys_data, std::array<int, kModelParam>& model_param,
                    StateData& state_data, InputData& input_data); 

// Scenario data
struct MPCConfig {
    int P;
    int M;
    int W; 

    Eigen::ArrayXf Q; // Container with dynamic size, not allocated yet
    Eigen::ArrayXf R; 
    float Ro; 
    bool bias_update;

    MPCConfig(); // Empty Default constructor
    MPCConfig(const json& sce_data, int n_CV, int n_MV); 
};

void ConstraintData(const json& sce_data, Eigen::ArrayXf& arr, bool upper);
void ParseScenarioData(const json& sce_data, std::string& system, MPCConfig& mpc_conf, 
                        Eigen::ArrayXf& upper_constraints, Eigen::ArrayXf& lower_constraints,
                        int n_CV, int n_MV);

void PrintContainer(std::array<int, 3> container);

#endif  // PARSE_H
