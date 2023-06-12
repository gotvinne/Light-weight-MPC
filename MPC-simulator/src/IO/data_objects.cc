/**
 * @file data_objects.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */

#include "IO/data_objects.h"
#include "IO/json_specifiers.h"

#include <stdexcept>
#include <iostream>

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

void CVData::PaddSData(json& s_data, int cv) {
    if (s_data.size() != n_MV_) {
        throw std::invalid_argument("CV number "+ std::to_string(cv)+" does not have n_MV step coefficients!");
    }
    for (int mv = 0; mv < n_MV_; mv++) {
        json response = s_data.at(mv); 
        int size = response.size();
        if (size > N_) {
            throw std::invalid_argument("N is wrongly defined, there exist longer s-responses!");
        } else if (size < N_) {
            double sn = *(response.end() - 1); // Accessing S(N)
            response.insert(response.end(), N_ - size, sn); 
            s_data.at(mv) = response;
        }
    }
}

CVData::CVData(const json& cv_data, int n_MV, int n_CV, int N) : n_CV_{n_CV}, n_MV_{n_MV}, N_{N} {
    int n_outputs = cv_data.size();
    if (n_outputs != n_CV) {
        throw std::invalid_argument("n_CV does not coincide with CV");
    }
    AllocateVectors();
    
    int i = 0; // cv counter
    for (auto& cv : cv_data) {
        outputs_.push_back(cv.at(kOutput));
        inits_.push_back(cv.at(kInit));
        units_.push_back(cv.at(kUnit));

        json s_data = cv.at(kS); // Fetching s coefficients
        PaddSData(s_data, i); // Padd if needed
        for (int mv = 0; mv < n_MV_; mv++) {
            FillSR(s_data, i, mv);
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
        json input_data = mv_data.at(inputs);
        Inputs.push_back(input_data.at(kInput));
        Inits.push_back(input_data.at(kInit));
        Units.push_back(input_data.at(kUnit));        
    }             
}

MPCConfig::MPCConfig() : P(), M(), W() {
    disable_slack = false;
}
MPCConfig::MPCConfig(const json& sce_data) {
    json mpc_data = sce_data.at(kMPC);
    P = mpc_data.at(kP);
    M = mpc_data.at(kM);
    W = mpc_data.at(kW);

    // Recall sizes
    int n_CV = int(mpc_data.at(kQ).size());
    int n_MV = int(mpc_data.at(kR).size());
    Q.resize(n_CV); 
    R.resize(n_MV);

    DetermineSlack(mpc_data, n_CV);
    // Store Q, Roh and RoL
    for (int i = 0; i < n_CV; i++) { 
        double q = mpc_data.at(kQ).at(i);
        if (q < 0) {
            throw std::invalid_argument("Negative Q-tuning");
        } else {
            Q[i] = q;
        }
        
        if (!disable_slack) {
            double roh = mpc_data.at(kRoH).at(i);
            double rol = mpc_data.at(kRoL).at(i);
            if (roh < 0 || rol < 0) {
                throw std::invalid_argument("Negative turning arguments in RoH and RoL");
            } else {
                RoH(i) = roh;
                RoL(i) = rol;
            }
        }
    }
    // Store R
    for (int i = 0; i < n_MV; i++) {
        double r = mpc_data.at(kR).at(i);
        if (r < 0) {
            throw std::invalid_argument("Negative R-tuning");
        } else {
            R[i] = r;
        }
    }
}

void MPCConfig::DetermineSlack(const json& mpc_data, int n_CV) {
    if (mpc_data.at(kRoH).empty() && mpc_data.at(kRoH).empty()) {
        disable_slack = true;
    } else {
        disable_slack = false;
        RoH.resize(n_CV);
        RoL.resize(n_CV);
    }
} 
