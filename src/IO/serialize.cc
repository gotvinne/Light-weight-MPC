/**
 * @file serialize.cc
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "IO/serialize.h"
#include "IO/json_specifiers.h"
#include "model/FSRModel.h"

#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

/**
 * @brief 
 * 
 * @param vector 
 * @param mat 
 * @param row 
 */
static void FillVector(json& vector, const MatrixXd& mat, int row) {
    for (int i = 0; i < mat.cols(); i++) {
        vector.push_back(mat(row, i));
    }
}

/**
 * @brief Formats the plain data in simulation file
 * 
 * @param data 
 * @param scenario 
 * @param fsr
 * @param T MPC horizon
 */
static void SerializeSimData(json& data, const string& scenario, const FSRModel& fsr, int T) {
    data[kScenario] = scenario;
    data[kT] = T;
    data[kN_CV] = fsr.getN_CV();
    data[kN_MV] = fsr.getN_MV(); 

    json du_tilde = json::array();
    for (int i = 0; i < fsr.getN_MV(); i++) {
        json row_arr = json::array();
        FillVector(row_arr, fsr.getDuTildeMat(), i);
        du_tilde.push_back(row_arr);
    }
    data[kDuTilde] = du_tilde;
}

/**
 * @brief Formats the CV data in the simulation file
 * 
 * @param data 
 * @param cv_data CDData object
 * @param z_min
 * @param z_max
 * @param n_CV Number of controlled variables
 * @param n_MV 
 */
static void SerializeSimCV(json& data, const CVData& cv_data, const MatrixXd& y_pred, const MatrixXd& z_min,
                 const MatrixXd& z_max, int n_CV, int n_MV) {
    json arr = json::array(); 
    
    std::vector<string> outputs = cv_data.getOutputs();
    std::vector<string> units = cv_data.getUnits();

    for (int i = 0; i < n_CV; i++) {
        json obj = json::object();
        obj[kOutput] = outputs[i];
        obj[kUnit] = units[i];

        obj[kC] = json::array({z_min(2 * n_MV + i), z_max(2 * n_MV + i)}); // Y constraints

        // Fill inn y
        json y_pred_vec = json::array();
        FillVector(y_pred_vec, y_pred, i);
        obj[kY_pred] = y_pred_vec;

        arr.push_back(obj);
    }
    data[kCV] = arr;
}

/**
 * @brief Formats the MV data in the simulation file
 * 
 * @param data 
 * @param mv_data MVData object
 * @param z_min
 * @param z_max
 * @param n_MV Number of manipulated variables
 */
static void SerializeSimMV(json& data, const MVData& mv_data, const MatrixXd& u, const VectorXd& z_min, const VectorXd& z_max, int n_MV) {
    json arr = json::array(); 

    for (int i = 0; i < n_MV; i++) {
        json obj = json::object();
        obj[kInput] = mv_data.Inputs[i];
        obj[kUnit] = mv_data.Units[i];

        obj[kC] = json::array({z_min(n_MV + i), z_max(n_MV + i)}); // U constraints
        
        json u_vec = json::array();
        FillVector(u_vec, u, i);
        obj[kU] = u_vec;

        arr.push_back(obj);
    }
    data[kMV] = arr;
}

void SerializeSimulation(json& data, const string& write_path, const string& scenario, const CVData& cv_data, const MVData& mv_data, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const VectorXd& z_min, const VectorXd& z_max, const FSRModel& fsr, int T) {
    SerializeSimData(data, scenario, fsr, T);
    SerializeSimCV(data, cv_data, y_pred, z_min, z_max, fsr.getN_CV(), fsr.getN_MV());
    SerializeSimMV(data, mv_data, u_mat, z_min, z_max, fsr.getN_MV());
    WriteJson(data, write_path);
}

void WriteJson(const json& data, const string& filepath) {
    std::ofstream ofs(filepath);
    ofs << data.dump(4) << std::endl;
    ofs.close();
}
