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
using VectorXf = Eigen::VectorXf;

/**
 * @brief C++ class representing state data described in system file
 */
class CVData {
    private:
    int n_CV_; /** Number of controlled variables */
    int n_MV_; /** Number of manipulated variables */
    int N_; /** Number of step response coefficients */
    
    std::vector<std::string> outputs_; /** vector of state specifiers */
    std::vector<float> inits_; /** vector of initial values */
    std::vector<std::string> units_; /** vector of corresponding state units */

    VectorXf** pp_SR_vec_; /** Matrix of Eigen::VectorXf holding every n_CV * n_MV step response */
    VectorXf y_ref_; /** Vector of state reference values */

    public:
    /**
     * @brief Empty Constructor. Construct a new CVData object.
     */
    CVData();

    /**
     * @brief Constructor. Construct a new CVData object. Allocating memory for pp_SR_vec
     * 
     * @param cv_data nlohmann::json object holding state system data
     * @param n_MV number of maipulated variables
     * @param n_CV number of controlled variables
     * @param N number of step coefficients
     * @param T mpc horizon
     */
    CVData(const json& cv_data, int n_MV, int n_CV, int N, int T);

    /**
     * @brief Destructor. Destroy the CVData object. Free memory of pp_SR_vec
     */
    ~CVData();

    /**
     * @brief 
     * 
     * @param s_data 
     */
    void FillSR(const json& s_data);

    /**
     * @brief Operator assignment, performing deep copying
     * 
     * @param rhs right hand operator of type CVData
     * @return CVData& 
     */
    CVData& operator=(const CVData& rhs);

    // Get functions
    VectorXf** getSR() const { return pp_SR_vec_; }
    VectorXf getYRef(int P, int k);
    std::vector<std::string> getOutputs() const { return outputs_; }
    std::vector<float> getInits() const { return inits_; }
    std::vector<std::string> getUnits() const { return units_; }
};

/**
 * @brief C++ struct representing input data 
 */
struct MVData {
    std::vector<std::string> Inputs; /** Vector containing input spesifiers */
    std::vector<float> Inits; /** Vector holding initial values */
    std::vector<std::string> Units; /** Vector of corresponding input units */

    /**
     * @brief Empty constructor. Construct a new MVData object
     */
    MVData();

    /**
     * @brief Construct a new MVData object
     * 
     * @param mv_data nhlomann::json object holding input system data
     * @param n_MV number of manipulated variables
     */
    MVData(const json& mv_data, int n_MV);
};

/**
 * @brief C++ struct representing the MPC spesifics described in the scenario file
 */
struct MPCConfig {
    int P; /** Prediction horison */
    int M; /** Control horizon */
    int W; /** Time delay coefficient */

    VectorXf Q; /** Output tuning */
    VectorXf R; /** Input change tuning */
    float Ro; /** Slack variable tuning */
    bool bias_update; /** Bias update / Integral effect enabled */

    /**
     * @brief Empty Constructor. Construct a new MPCConfig object.
     */
    MPCConfig();

    /**
     * @brief Construct a new MPCConfig object
     * 
     * @param sce_data nlohmann::json object holding scenario data
     * @param n_CV number of controlled variables
     * @param n_MV number of manipulated variables
     */
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
void FillReference(const json& ref_data, VectorXf& ref, int start_index, int interval);

#endif // DATA_OBJECTS_H
