/**
 * @file data_objects.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "data_objects.h"
#include "json_specifiers.h"

#include <vector>
#include <stdexcept>
#include <iostream>

#include <nlohmann/json.hpp>
#include <Eigen/Dense>

/* Error handling:
    Check that S is correct with N_MV and N_CV
    Check that number of S is correct with n_CV
*/
CVData::CVData() {}
CVData::CVData(const json& cv_data, int n_MV, int n_CV, int N, int T) : n_CV_{n_CV}, n_MV_{n_MV}, N_{N} {
    int n_outputs = cv_data.size();
    if (n_outputs != n_CV) {
        throw std::invalid_argument("n_CV does not coincide with CV");
    }
    y_ref_.resize(T*n_MV);
    // Allocate matrix of Eigen::VectorXd
    pp_SR_vec_ = new VectorXd*[n_CV_];
    for (int i = 0; i < n_CV_; ++i) {
        pp_SR_vec_[i] = new VectorXd[n_MV_];
    }
    for (int row = 0; row < n_CV_; row++) {
        for (int col = 0; col < n_MV_; col++) {
            pp_SR_vec_[row][col] = VectorXd::Zero(N);
        }
    }
    for (int outputs = 0; outputs < n_CV; outputs++) {
        json output_data = cv_data.at(outputs); //Selecting one output
        outputs_.push_back(output_data.at(kOutput));
        inits_.push_back(output_data.at(kInit));
        units_.push_back(output_data.at(kUnit));

        if (output_data.at(kY_Ref).size() < T) {
            throw std::invalid_argument("Too few input data for the horizon");
        }
        FillReference(output_data.at(kY_Ref), y_ref_, T*outputs, T);
        FillSR(output_data.at(kS));
    }                          
}

CVData::~CVData() {
    for (int i = 0 ; i < n_CV_; i++) {
        delete[] pp_SR_vec_[i];
    }
    delete[] pp_SR_vec_;
}

CVData& CVData::operator=(const CVData& rhs) {
    n_CV_ = rhs.n_CV_;
    n_MV_ = rhs.n_MV_;
    N_ = rhs.N_;
    outputs_ = rhs.outputs_;
    inits_ = rhs.inits_;
    units_ = rhs.units_;
    y_ref_ = rhs.y_ref_;

    // Deep copying
    pp_SR_vec_ = new VectorXd*[n_CV_];
    for (int i = 0; i < n_CV_; ++i) {
        pp_SR_vec_[i] = new VectorXd[n_MV_];
    }
    for (int row = 0; row < n_CV_; row++) {
        for (int col = 0; col < n_MV_; col++) {
            pp_SR_vec_[row][col] = rhs.pp_SR_vec_[row][col];
        }
    }
    return *this;
}

void FillReference(const json& ref_data, VectorXd& ref, int start_index, int interval) {
    for (int i = start_index; i < start_index+interval; i++) {
            ref[i] = ref_data.at(i);
    }  
}

void CVData::FillSR(const json& s_data) {
    for (int i = 0; i < n_CV_; i++) {
        for (int j = 0; j < n_MV_; j++) {
            VectorXd vec = VectorXd::Zero(N_);
            for (int k = 0; k < N_; k++) {
                vec(k) = s_data.at(i).at(k);
            }
            pp_SR_vec_[i][j] = vec;
        }
    }       
}

VectorXd CVData::getYRef(int P, int k) {
    return y_ref_(Eigen::seq(k, P+k));
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
