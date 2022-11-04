/**
 * @file data_objects.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef DATA_OBJECTS_H
#define DATA_OBJECTS_H

#include <vector>
#include <string>

#include <nlohmann/json.hpp>
#include <Eigen/Dense>

using json = nlohmann::json; 

/**
 * @brief C++ struct representing state data described in system file
 * 
 */
class CVData {
    private:
    int n_CV_;
    int n_MV_; 
    int N_;
    
    std::vector<std::string> outputs_; 
    std::vector<float> inits_;
    std::vector<std::string> units_;

    Eigen::VectorXf** pp_SR_vec_;
    Eigen::VectorXf y_ref_;

    public:
    CVData();
    CVData(const json& cv_data, int n_MV, int n_CV, int N, int T);
    ~CVData();

    void FillSR(const json& s_data);
    CVData& operator=(const CVData& rhs);
    Eigen::VectorXf** getSR() const { return pp_SR_vec_; }
    Eigen::VectorXf getYRef(int P, int k);
};

/**
 * @brief C++ struct representing input data 
 * 
 */
struct MVData {
    std::vector<std::string> Inputs; 
    std::vector<float> Inits;
    std::vector<std::string> Units;

    MVData();
    MVData(const json& mv_data, int n_MV);
};

/**
 * @brief C++ struct representing the MPC spesifics described in the scenario file
 * 
 */
struct MPCConfig {
    int P;
    int M;
    int W; 

    Eigen::VectorXf Q; // Container with dynamic size, not allocated yet
    Eigen::VectorXf R; 
    float Ro; 
    bool bias_update;

    MPCConfig();
    MPCConfig(const json& sce_data, int n_CV, int n_MV); 
};

/**
 * @brief Assigns reference values to the Eigen::VectorXf passed by reference
 * 
 * @param ref_data json object holding input reference data
 * @param ref Eigen::VectorXf
 * @param start_index index to start to fill from
 * @param interval number of values to be filled
 */
void FillReference(const json& ref_data, Eigen::VectorXf& ref, int start_index, int interval);

#endif // DATA_OBJECTS_H
