/**
 * @file data_objects.h
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */

#ifndef DATA_OBJECTS_H
#define DATA_OBJECTS_H

#include <vector>
#include <string>

#include <nlohmann/json.hpp>
#include <Eigen/Dense>
using json = nlohmann::json; 
using VectorXd = Eigen::VectorXd;
using string = std::string;

/**
 * @brief C++ class representing state data described in system file
 */
class CVData {
private:
    int n_CV_; /** Number of controlled variables */
    int n_MV_; /** Number of manipulated variables */
    int N_; /** Largest number of step response coefficients */
    
    std::vector<string> outputs_; /** vector of state specifiers */
    std::vector<double> inits_; /** vector of initial values */
    std::vector<string> units_; /** vector of corresponding state units */

    VectorXd** pp_SR_vec_; /** Matrix of Eigen::VectorXd holding every n_CV * n_MV step response */
    
     /**
     * @brief Filling the dobbel linked VectorXd
     * 
     * @param s_data Step response coefficients in JSON format
     * @param cv cv index
     * @param mv mv index
     */
    void FillSR(const json& s_data, int cv, int mv);

    /**
     * @brief Helper function, allocating Eigen::VectorXd in constuctor
     * 
     */
    void AllocateVectors();

    /**
     * @brief Padding S coefficients such that the model representation has equal number of Ss for each response
     * 
     * @param s_data Step coefficients parsed from model
     * @param cv cv index
     */
    void PaddSData(json& s_data, int cv); 

public:
    /**
     * @brief Empty Constructor. Construct an empty object only allocating data.
     */
    CVData() : n_CV_{0}, n_MV_{0} {}

    /**
     * @brief Constructor. Construct a new CVData object. Allocating memory for pp_SR_vec
     * 
     * @param cv_data nlohmann::json object holding state system data
     * @param n_MV number of maipulated variables
     * @param n_CV number of controlled variables
     * @param N number of step coefficients
     */
    CVData(const json& cv_data, int n_MV, int n_CV, int N);

    /**
     * @brief Destructor. Destroy the CVData object. Free memory of pp_SR_vec
     */
    ~CVData();

    /**
     * @brief Operator assignment, performing deep copying
     * 
     * @param rhs right hand operator of type CVData
     * @return CVData& 
     */
    CVData& operator=(const CVData& rhs);

    void setInits(double value, int index) { inits_.at(index) = value; }

    // Get functions
    VectorXd** getSR() const { return pp_SR_vec_; }
    std::vector<string> getOutputs() const { return outputs_; }
    std::vector<double> getInits() const { return inits_; }
    std::vector<string> getUnits() const { return units_; }
};

/**
 * @brief C++ struct representing input data 
 */
struct MVData {
    std::vector<string> Inputs; /** Vector containing input spesifiers */
    std::vector<double> Inits; /** Vector holding initial values */
    std::vector<string> Units; /** Vector of corresponding input units */

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

    void setInits(double value, int index) { Inits.at(index) = value; }
};

/**
 * @brief C++ struct representing the MPC spesifics described in the scenario file
 */
struct MPCConfig {
    int P; /** Prediction horison */
    int M; /** Control horizon */
    int W; /** Time delay coefficient */

    VectorXd Q; /** Output tuning */
    VectorXd R; /** Input change tuning */
    VectorXd RoH; /** Upper Slack variable tuning */
    VectorXd RoL; /** Lower Slack variable tuning */
    bool disable_slack;

    /**
     * @brief Empty Constructor. Construct a new MPCConfig object.
     */
    MPCConfig();

    /**
     * @brief Construct a new MPCConfig object
     * 
     * @param sce_data nlohmann::json object holding scenario data
     */
    MPCConfig(const json& sce_data);

    /**
     * @brief Determine if slack variables shall be simulated
     *  
     * @param mpc_data json data holding mpc configuration
     * @param n_CV number of constrained variables
     */
    void DetermineSlack(const json& mpc_data, int n_CV); 
};

#endif // DATA_OBJECTS_H
