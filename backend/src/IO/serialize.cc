/**
 * @file serialize.cc
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright  Released under the terms of the BSD 3-Clause License
 * 
 */

#include "IO/serialize.h"
#include "IO/parse.h" 
#include "IO/json_specifiers.h"
#include "model/FSRModel.h"

#include <fstream>
#include <vector>
#include <filesystem>
#include <stdexcept>

/**
 * @brief Fill json::array() object from Eigen::MatrixXd
 * 
 * @param vector Vector to be filled
 * @param mat data matrix
 * @param row row indicator
 */
static void FillVector(json& vector, const MatrixXd& mat, int row) {
    for (int i = 0; i < mat.cols(); i++) {
        vector.push_back(mat(row, i));
    }
}

static json SliceVector(json& vector, int start, int end) {
    json vec = json::array();

    if (start > end) {
        std::invalid_argument("Start index is higher than end index");
    } if (end > vector.size()) {
        std::out_of_range("End index is out of range");
    }
    for (int i = start; i < end; i++) {
        vec.push_back(vector[i]);
    }
    return vec;
}

/**
 * @brief Formats the plain data in simulation file
 * 
 * @param data json object
 * @param scenario scenario to serialize
 * @param fsr FSRModel
 * @param T MPC horizon
 */
static void SerializeSimData(json& data, const string& scenario, const FSRModel& fsr, int T) {
    data[kScenario] = scenario;
    data[kT] = T;
    data[kN_CV] = fsr.getN_CV();
    data[kN_MV] = fsr.getN_MV(); 
    data[kP] = fsr.getP();

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
 * @param data json data
 * @param cvd CDData object
 * @param z_min lower constraints
 * @param z_max upper constraints
 * @param ref
 * @param n_CV Number of controlled variables
 * @param n_MV Number of Manipulated variables
 */
static void SerializeSimCV(json& data, const CVData& cvd, const MatrixXd& y_pred, const MatrixXd& z_min,
                 const MatrixXd& z_max, const MatrixXd& ref, int n_CV, int n_MV) {
    json arr = json::array(); 
    std::vector<string> outputs = cvd.getOutputs();
    std::vector<string> units = cvd.getUnits();

    for (int i = 0; i < n_CV; i++) {
        json obj = json::object();
        obj[kOutput] = outputs[i];
        obj[kUnit] = units[i];

        // Fill ref
        json ref_vec = json::array();
        FillVector(ref_vec, ref, i);
        obj[kRef] = ref_vec;

        obj[kC] = json::array({z_min(2 * n_MV + i), z_max(2 * n_MV + i)}); // Y constraints

        // Fill y
        json y_pred_vec = json::array();
        FillVector(y_pred_vec, y_pred, i);
        obj[kY_pred] = y_pred_vec;

        arr.push_back(obj);
    }
    data[kCV] = arr;
}

// Open loop serialization
static void SerializeSimCV(json& data, const CVData& cvd, const MatrixXd& y_pred, int n_CV) {
    json arr = json::array(); 
    
    std::vector<string> outputs = cvd.getOutputs();
    std::vector<string> units = cvd.getUnits();

    for (int i = 0; i < n_CV; i++) {
        json obj = json::object();
        obj[kOutput] = outputs[i];
        obj[kUnit] = units[i];

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
 * @param data json data
 * @param mvd MVData 
 * @param z_min lower constraints
 * @param z_max upper constraints
 * @param n_MV Number of manipulated variables
 */
static void SerializeSimMV(json& data, const MVData& mvd, const MatrixXd& u, const VectorXd& z_min, const VectorXd& z_max, int n_MV) {
    json arr = json::array(); 

    for (int i = 0; i < n_MV; i++) {
        json obj = json::object();
        obj[kInput] = mvd.Inputs[i];
        obj[kUnit] = mvd.Units[i];

        obj[kC] = json::array({z_min(n_MV + i), z_max(n_MV + i)}); // U constraints
        
        json u_vec = json::array();
        FillVector(u_vec, u, i);
        obj[kU] = u_vec;

        arr.push_back(obj);
    }
    data[kMV] = arr;
}

static void SerializeSimMV(json& data, const MVData& mvd, const MatrixXd& u, int n_MV) {
    json arr = json::array(); 

    for (int i = 0; i < n_MV; i++) {
        json obj = json::object();
        obj[kInput] = mvd.Inputs[i];
        obj[kUnit] = mvd.Units[i];
        
        json u_vec = json::array();
        FillVector(u_vec, u, i);
        obj[kU] = u_vec;

        arr.push_back(obj);
    }
    data[kMV] = arr;
}

/**
 * @brief Serialize constraint data
 * 
 * @param data json data
 * @param l_du lower du constraint
 * @param l_u lower u constraint
 * @param l_y lower y constraint
 * @param u_du upper du constraint
 * @param u_u upper u constraint
 * @param u_y upper y constraint
 * @param n_CV number of controlled variables 
 * @param n_MV number of manipulated variables
 */
static void SerializeConstraints(json& data, const VectorXd& l_du, const VectorXd& l_u,
                    const VectorXd& l_y, const VectorXd& u_du, const VectorXd& u_u,
                    const VectorXd& u_y, int n_CV, int n_MV) {
    json arr = json::array();

    for (int i = 0; i < n_MV; i++) {
        json c_obj = json::object();
        c_obj[kDu + "[" + std::to_string(i) + "]"] = json::array({l_du(i), u_du(i)});
        arr.push_back(c_obj);
    }

    for (int i = 0; i < n_MV; i++) {
        json c_obj = json::object();
        c_obj[kU + "[" + std::to_string(i) + "]"] = json::array({l_u(i), u_u(i)});
        arr.push_back(c_obj);
    }

    for (int i = 0; i < n_CV; i++) {
        json c_obj = json::object();
        c_obj[kY + "[" + std::to_string(i) + "]"] = json::array({l_y(i), u_y(i)});
        arr.push_back(c_obj);
    }
    data[kC] = arr;
}

/**
 * @brief 
 * 
 * @param data 
 * @param mpc_m 
 * @param Q 
 * @param R 
 * @param Ro 
 * @param bias_update 
 */
static void SerializeMPC(json& data, std::map<string, int> mpc_m, const VectorXd& Q, const VectorXd& R, const VectorXd& Ro, bool bias_update) {
    json obj = json::object();
    obj[kP] = mpc_m[kP];
    obj[kM] = mpc_m[kM];
    obj[kW] = mpc_m[kW];
    obj[kBu] = bias_update;

    json q = json::array(), r = json::array(), rol = json::array(), roh = json::array();
    for (int i = 0; i < Q.rows(); i++) {
        q.push_back(Q(i));
        rol.push_back(Ro(0, i));
        roh.push_back(Ro(1, i));
    }
    for (int i = 0; i < R.rows(); i++) {
        r.push_back(R(i));
    }

    obj[kQ] = q;
    obj[kR] = r;
    obj[kRoL] = rol;
    obj[kRoH] = roh;

    data[kMPC] = obj;
}

void WriteJson(const json& data, const string& filepath) {
    std::ofstream ofs(filepath);
    ofs << data.dump(4) << std::endl;
    ofs.close();
}

//////////////////////////////////
//** SERIALIZE SIMULATION FILE *// 
//////////////////////////////////

// Serialize for new simulation
void SerializeSimulationNew(const string& write_path, const string& scenario, const CVData& cvd, const MVData& mvd, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const VectorXd& z_min, const VectorXd& z_max, 
                    const MatrixXd& ref, const FSRModel& fsr, int T) {
    json data;
    SerializeSimData(data, scenario, fsr, T);
    SerializeSimCV(data, cvd, y_pred, z_min, z_max, ref, fsr.getN_CV(), fsr.getN_MV());
    SerializeSimMV(data, mvd, u_mat, z_min, z_max, fsr.getN_MV());
    WriteJson(data, write_path);
}

// Update simulation file for further simulation
void SerializeSimulation(const string& write_path, const MatrixXd& y_pred, const MatrixXd& u_mat, 
                        const MatrixXd& ref, int T) {
    json sim_data = ReadJson(write_path); // Assume this file already exists.
    int old_P = int(sim_data.at(kP));
    int old_T = int(sim_data.at(kT));

    sim_data[kT] = old_T + T; // Update MPC horizon
    
    json cv_data = sim_data.at(kCV), mv_data = sim_data.at(kMV);
    int i = 0;
    for (auto& cv : cv_data) {
        json old_predictions = SliceVector(cv[kY_pred], 0, old_T); // Slice away Predictions
        FillVector(old_predictions, y_pred, i); // Update Y_pred
        cv[kY_pred] = old_predictions;

        json old_ref = SliceVector(cv[kRef], 0, old_T); // Update reference
        FillVector(old_ref, ref, i);
        cv[kRef] = old_ref; 
        i++;
    }
    i = 0;
    for (auto& mv : mv_data) {
        json old_actuations = SliceVector(mv[kU], 0, old_T);
        FillVector(old_actuations, u_mat, i); // Update U
        mv[kU] = old_actuations; 
        i++;
    }

    // Write updated variables back
    sim_data[kCV] = cv_data;
    sim_data[kMV] = mv_data;
    WriteJson(sim_data, write_path);
}

// Serialization for Web application
string SerializeSimulation(const string& scenario, const CVData& cvd, const MVData& mvd, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const VectorXd& z_min, const VectorXd& z_max,
                    const MatrixXd& ref, const FSRModel& fsr, int T) {
    json data;
    SerializeSimData(data, scenario, fsr, T);
    SerializeSimCV(data, cvd, y_pred, z_min, z_max, ref, fsr.getN_CV(), fsr.getN_MV());
    SerializeSimMV(data, mvd, u_mat, z_min, z_max, fsr.getN_MV());
    return to_string(data);
}

void SerializeOpenLoop(const string& write_path, const string& scenario, const CVData& cvd, const MVData& mvd, 
                    const MatrixXd& y_pred, const MatrixXd& u_mat, const FSRModel& fsr, int T) {
    json data;
    SerializeSimData(data, scenario, fsr, T);
    SerializeSimCV(data, cvd, y_pred, fsr.getN_CV());
    SerializeSimMV(data, mvd, u_mat, fsr.getN_MV());
    WriteJson(data, write_path);
}

////////////////////////////////
//** SERIALIZE SCENARIO FILE *//
////////////////////////////////

void SerializeScenario(const string& write_path, const string& scenario, const string& system, const string& sys_path, std::map<string, int> mpc_m,
                     const VectorXd& Q, const VectorXd& R, const VectorXd& Ro, bool bias_update, const VectorXd& l_du, 
                     const VectorXd& l_u, const VectorXd& l_y, const VectorXd& u_du, const VectorXd& u_u, const VectorXd& u_y,
                     int n_CV, int n_MV) {
    json data;
    if (Q.rows() != n_CV) {
        throw std::out_of_range("Q matrix dimension does not match system description");
    }
    if (R.rows() != n_MV) {
        throw std::out_of_range("R matrix dimension does not match system description");
    }
    if (Ro.cols() != n_CV) {
        throw std::out_of_range("Number of Ro elements does not match system description");
    }

    if (l_du.rows() != u_du.rows() || l_du.rows() != n_MV) {
        throw std::out_of_range("Number of delta U constraints does not match system description");
    }
    if (l_u.rows() != u_u.rows() || l_u.rows() != n_MV) {
        throw std::out_of_range("Number of U constraints does not match system description");
    }
    if (l_y.rows() != u_y.rows() || l_y.rows() != n_CV) {
        throw std::out_of_range("Number of Y constraints does not match system description");
    }

    data[kSystem] = system; // Write system
    SerializeMPC(data, mpc_m, Q, R, Ro, bias_update);
    SerializeConstraints(data, l_du, l_u, l_y, u_du, u_u, u_y, n_CV, n_MV);
    WriteJson(data, write_path);
}

