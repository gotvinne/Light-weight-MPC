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
using VectorXd = Eigen::VectorXd;

/**
 * @brief C++ class representing state data described in system file
 */
class CVData {
private:
    int n_CV_; /** Number of controlled variables */
    int n_MV_; /** Number of manipulated variables */
    int N_; /** Number of step response coefficients */
    
    std::vector<std::string> outputs_; /** vector of state specifiers */
    std::vector<double> inits_; /** vector of initial values */
    std::vector<std::string> units_; /** vector of corresponding state units */

    VectorXd** pp_SR_vec_; /** Matrix of Eigen::VectorXd holding every n_CV * n_MV step response */
    VectorXd* y_ref_; /** Matrix of state reference values (n_CV * T), the vector is Populated P times*/

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
     * @param T mpc horizon
     * @param P Predition horizon
     */
    CVData(const json& cv_data, int n_MV, int n_CV, int N, int T, int P);

    /**
     * @brief Destructor. Destroy the CVData object. Free memory of pp_SR_vec
     */
    ~CVData();

    /**
     * @brief Filling the dobbel linked VectorXd
     * 
     * @param s_data 
     * @param cv 
     * @param mv
     */
    void FillSR(const json& s_data, int cv, int mv);

    /**
     * @brief Helper function, allocating Eigen::VectorXd in constuctor
     * 
     * @param size used to allocate y_ref
     */
    void AllocateVectors(int size);

    /**
     * @brief Operator assignment, performing deep copying
     * 
     * @param rhs right hand operator of type CVData
     * @return CVData& 
     */
    CVData& operator=(const CVData& rhs);

    void PopulateRef(int P, int T);

    // Get functions
    VectorXd** getSR() const { return pp_SR_vec_; }
    VectorXd* getYRef() const { return y_ref_; }
    std::vector<std::string> getOutputs() const { return outputs_; }
    std::vector<double> getInits() const { return inits_; }
    std::vector<std::string> getUnits() const { return units_; }

    void PrintYRef();
};

/**
 * @brief C++ struct representing input data 
 */
struct MVData {
    std::vector<std::string> Inputs; /** Vector containing input spesifiers */
    std::vector<double> Inits; /** Vector holding initial values */
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

    VectorXd Q; /** Output tuning */
    VectorXd R; /** Input change tuning */
    double RoU; /** Upper Slack variable tuning */
    double RoL; /** Lower Slack variable tuning */
    bool bias_update; /** Bias update / Integral effect enabled */

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
};

#endif // DATA_OBJECTS_H
