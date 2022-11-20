/**
 * @file formatting
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#include "formatting.h"
#include "json_specifiers.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json; 
using MatrixXd = Eigen::MatrixXd;
using string = std::string; 

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

void WriteJson(const json& data, const string& filepath) {
    std::ofstream ofs(filepath);
    ofs << data.dump(4) << std::endl;
    ofs.close();
}

void FormatSimData(json& data, const string& filepath, const string& scenario,
                 int n_CV, int n_MV, int T) {
    data[kScenario] = scenario;
    data[kT] = T;
    data[kN_CV] = n_CV;
    data[kN_MV] = n_MV; 
}

void FormatSimCV(json& data, const CVData& cv_data, const MatrixXd& y_pred, int n_CV) {
    json arr = json::array(); 
    
    std::vector<std::string> outputs = cv_data.getOutputs();
    std::vector<std::string> units = cv_data.getUnits();

    for (int i = 0; i < n_CV; i++) {
        json obj = json::object();
        obj[kOutput] = outputs[i];
        obj[kUnit] = units[i];

        // Fill inn c, y
        json y_pred_vec = json::array();
        FillVector(y_pred_vec, y_pred, i);
        obj[kY_pred] = y_pred_vec;

        arr.push_back(obj);
    }
    data[kCV] = arr;
}

void FormatSimMV(json& data, const MVData& mv_data, const MatrixXd& u, int n_MV) {
    json arr = json::array(); 

    for (int i = 0; i < n_MV; i++) {
        json obj = json::object();
        obj[kInput] = mv_data.Inputs[i];
        obj[kUnit] = mv_data.Units[i];
        
        // Fill inn c
        json u_vec = json::array();
        FillVector(u_vec, u, i);
        obj[kU] = u_vec;

        arr.push_back(obj);
    }
    data[kMV] = arr;
}

void FormatScenario(json& data, const string& write_path, const string& scenario, const CVData& cv_data, const MVData& mv_data, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, int n_CV, int n_MV, int T) {
    FormatSimMV(data, mv_data, u_mat, n_MV);
    FormatSimCV(data, cv_data, y_pred, n_CV);
    FormatSimData(data, write_path, scenario, T, n_CV, n_MV);
    WriteJson(data, write_path);
}

