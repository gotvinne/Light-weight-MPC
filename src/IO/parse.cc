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
#include <map>
#include <iostream>
#include <fstream>
#include <vector>

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
/* Error handling:
    Check that S is correct with N_MV and N_CV
    Check that number of S is correct with n_CV
*/

StateData::StateData(const json& cv_data, int n_MV, int n_CV, int N) {
    int n_states = cv_data.size();

    S.resize(n_MV, N*n_CV);
    for (int states = 0; states < n_CV; states++) {
        json state_data = cv_data.at(states); //Selecting one state
        State.push_back(state_data.at(kState));
        Init.push_back(state_data.at(kInit));

    }
        for (int i = 0; i < n_MV; i++) {
            for (int j = 0; j < N; j++) {
                S(i,j) = cv_data.at(kS).at(i).at(j);
            }
        }                             
}

//void FillStateCoMatrix(const json& data, Eigen::MatrixXf S, )

// Need to customize input data, to generate datapoints based on mpc_horizon.
// Check that n_inputs == n_MV!
// Check that T < then number of reference points in data. 
InputData::InputData(const json& mv_data, int n_MV, int T) {
    int n_inputs = mv_data.size();

    Ref.resize(T*n_MV);
    for (int inputs = 0; inputs < n_MV; inputs++) {
        json input_data = mv_data.at(inputs); // Selecting one input
        Input.push_back(input_data.at(kInput));
        Init.push_back(input_data.at(kInit));

        FillReference(input_data.at(kU), Ref, T*inputs, T);
    }             
}

void FillReference(const json& data, Eigen::ArrayXf& ref, int start_index, int interval) {
    for (int i = start_index; i < (start_index+1)*interval; i++) {
            ref[i] = data.at(i);
    }  
}

MPCConfig::MPCConfig() : P(), M(), W(), Ro(), bias_update() {}
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

void ConstraintData(const json& sce_data, Eigen::ArrayXf& arr, bool upper) {
    try {
        json j_arr(sce_data.at(kC));
        int size = j_arr.size();

        arr.resize(size);
        for (int i = 0; i < size; i++) {
            for (auto& elem : j_arr.at(i).items()) {
                arr[i] = elem.value().at(upper);
            }
        }
    }
    catch(json::exception& e) {
        std::cout << e.what() << std::endl; 
    } 
}

void ParseScenarioData(const json& sce_data, std::string& system, MPCConfig& mpc_config, 
                        Eigen::ArrayXf& upper_constraints, Eigen::ArrayXf& lower_constraints,
                        int n_CV, int n_MV) {
    system = sce_data.at(kSystem);
    mpc_config = MPCConfig(sce_data, n_CV, n_MV);
    ConstraintData(sce_data, upper_constraints, true);
    ConstraintData(sce_data, lower_constraints, false);
}

void ParseSystemData(const json& sys_data, std::map<std::string, int>& model_param,
                    StateData& state_data, InputData& input_data) {
    ModelData(sys_data, model_param);

    //input_data(sys_data.at(kMV), ) 

}

void PrintContainer(std::map<std::string,int> container) {
    for (auto ptr = container.begin(); ptr != container.end(); ptr++) {
        std::cout << ptr->first << ", " << ptr->second << std::endl;
    }
}
