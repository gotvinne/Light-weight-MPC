/**
 * @file data_structs.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "data_structs.h"
#include "json_specifiers.h"

#include <vector>
#include <stdexcept>

/* Error handling:
    Check that S is correct with N_MV and N_CV
    Check that number of S is correct with n_CV
*/
CVData::CVData() {}
CVData::CVData(const json& cv_data, int n_MV, int n_CV, int N, int T) {
    int n_outputs = cv_data.size();
    if (n_outputs != n_CV) {
        throw std::invalid_argument("n_CV does not coincide with CV");
    }
    Y_Ref.resize(T*n_MV);

    // Allocate matrix of Eigen::VectorXf
    S = new Eigen::VectorXf*[n_CV];
    for (int i = 0; i < n_CV; ++i) {
        S[i] = new Eigen::VectorXf[n_MV];
    }

    for (int row = 0; row < n_CV; row++) {
        for (int col = 0; col < n_MV; col++) {
            S[row][col] = Eigen::VectorXf::Zero(N);
        }
    }
    
    for (int outputs = 0; outputs < n_CV; outputs++) {
        json output_data = cv_data.at(outputs); //Selecting one output
        Outputs.push_back(output_data.at(kOutput));
        Inits.push_back(output_data.at(kInit));
        Units.push_back(output_data.at(kUnit));

        if (output_data.at(kY_Ref).size() < T) {
            throw std::invalid_argument("Too few input data for the horizon");
        }
        FillReference(output_data.at(kY_Ref), Y_Ref, T*outputs, T);
        //FillStepCoMatrix(output_data.at(kS), S, n_MV, outputs*N, N);
    }                          
}

CVData::~CVData() {
    for (int i = 0 ; i < n_CV_; i++) {
        delete[] S[i];
    }
    delete[] S;
}

void FillReference(const json& ref_data, Eigen::VectorXf& ref, int start_index, int interval) {
    for (int i = start_index; i < start_index+interval; i++) {
            ref[i] = ref_data.at(i);
    }  
}

void FillStepCoMatrix(const json& s_data, Eigen::MatrixXf& S, int n_MV, int start_index, int interval) {
    for (int i = 0; i < n_MV; i++) {
        for (int j = 0; j < interval; j++) {
            S(i,start_index+j) = s_data.at(i).at(j);
        }
    }       
}

MVData::MVData() {}
MVData::MVData(const json& mv_data, int n_MV) {
    int n_inputs = mv_data.size();
    if (n_inputs != n_MV) {
        throw std::invalid_argument("n_MV does not coincide with MV");
    }
    
    for (int inputs = 0; inputs < n_MV; inputs++) {
        json input_data = mv_data.at(inputs); // Selecting one input
        Inputs.push_back(input_data.at(kInput));
        Inits.push_back(input_data.at(kInit));
        Units.push_back(input_data.at(kUnit));        
    }             
}

MPCConfig::MPCConfig() : P(), M(), W(), Ro(), bias_update() {}
MPCConfig::MPCConfig(const json& sce_data, int n_CV, int n_MV) {
    json mpc_data = sce_data.at(kMPC);
    P = mpc_data.at(kP);
    M = mpc_data.at(kM);
    W = mpc_data.at(kW);

    Q.resize((P-W)*n_CV); 
    R.resize(M*n_MV);
    // Implement size check
    for (int i = 0; i < ((P-W)*n_CV); i++) {
        Q[i] = mpc_data.at(kQ).at(i);
    }
    for (int i = 0; i < M*n_MV; i++) {
        R[i] = mpc_data.at(kR).at(i);
    }

    Ro = mpc_data.at(kRo);
    bias_update = mpc_data.at(kBu);
}