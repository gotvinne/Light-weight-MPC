/**
 * @file data_objects.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "IO/data_objects.h"
#include "IO/json_specifiers.h"

#include <vector>
#include <stdexcept>
#include <iostream>

#include <nlohmann/json.hpp>
#include <Eigen/Dense>

/**
 * @brief Convert a nlohmann::json::array to Eigen::VectorXd
 * 
 * @param vec Eigen::VectorXD
 * @param arr nlohmann::json::array
 */
static void EigenFromJson(VectorXd& vec, const json& arr) {
    int i = 0;
    for (auto& elem : arr) {
        vec(i++) = (double) elem;
    }
}

/* Error handling:
    Check that S is correct with N_MV and N_CV
    Check that number of S is correct with n_CV
*/
CVData::CVData(const json& cv_data, int n_MV, int n_CV, int N) : n_CV_{n_CV}, n_MV_{n_MV}, N_{N} {
    int n_outputs = cv_data.size();
    if (n_outputs != n_CV) {
        throw std::invalid_argument("n_CV does not coincide with CV");
    }
    AllocateVectors();
    
    int i = 0;
    for (auto& cv : cv_data) {
        outputs_.push_back(cv.at(kOutput));
        inits_.push_back(cv.at(kInit));
        units_.push_back(cv.at(kUnit));

        for (int mv = 0; mv < n_MV_; mv++) {
            FillSR(cv.at(kS), i, mv);
        }
        i++;
    }                          
}

CVData::~CVData() {
    if (n_CV_ != 0 && n_MV_ != 0) {
        for (int i = 0 ; i < n_CV_; i++) {
            delete[] pp_SR_vec_[i];
        }
        delete[] pp_SR_vec_;
    }
}

void CVData::AllocateVectors() {
    // Allocate matrix of Eigen::VectorXd
    pp_SR_vec_ = new VectorXd*[n_CV_];
    for (int i = 0; i < n_CV_; i++) {
        pp_SR_vec_[i] = new VectorXd[n_MV_];
    }
    for (int row = 0; row < n_CV_; row++) {
        for (int col = 0; col < n_MV_; col++) {
            pp_SR_vec_[row][col] = VectorXd::Zero(N_);
        }
    }
}

CVData& CVData::operator=(const CVData& rhs) {
    // Implemented without deep-copying. Since we assume default constructor is called first. 
    n_CV_ = rhs.n_CV_;
    n_MV_ = rhs.n_MV_;
    N_ = rhs.N_;
    outputs_ = rhs.outputs_;
    inits_ = rhs.inits_;
    units_ = rhs.units_;

    // Deep copying
    pp_SR_vec_ = new VectorXd*[n_CV_];
    for (int cv = 0; cv < n_CV_; cv++) {
        pp_SR_vec_[cv] = new VectorXd[n_MV_];
    }
    for (int row = 0; row < n_CV_; row++) {
        for (int col = 0; col < n_MV_; col++) {
            pp_SR_vec_[row][col] = rhs.pp_SR_vec_[row][col];
        }
    }
    return *this;
}

void CVData::FillSR(const json& s_data, int cv, int mv) {
    VectorXd vec = VectorXd::Zero(N_);
    EigenFromJson(vec, s_data.at(mv));
    pp_SR_vec_[cv][mv] = vec;    
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

MPCConfig::MPCConfig() : P(), M(), W(), RoU(), RoL(), bias_update() {}
MPCConfig::MPCConfig(const json& sce_data) {
    json mpc_data = sce_data.at(kMPC);
    P = mpc_data.at(kP);
    M = mpc_data.at(kM);
    W = mpc_data.at(kW);

    int n_CV = int(mpc_data.at(kQ).size());
    int n_MV = int(mpc_data.at(kR).size());

    Q.resize(n_CV); 
    R.resize(n_MV);
    RoU.resize(n_CV);
    RoL.resize()
    
    for (int i = 0; i < n_CV; i++) { 
        Q[i] = mpc_data.at(kQ).at(i);
    }
   
    for (int i = 0; i < n_MV; i++) {
        R[i] = mpc_data.at(kR).at(i);
    }
    
    RoU = mpc_data.at(kRoU);
    RoL = mpc_data.at(kRoL);
    bias_update = mpc_data.at(kBu);
}
