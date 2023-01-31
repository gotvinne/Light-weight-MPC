/**
 * @file lightweight_MPC.h
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LIGHTWEIGHT_MPC_H
#define LIGHTWEIGHT_MPC_H

#include "IO/serialize.h"
#include "IO/parse.h"
#include "IO/data_objects.h"
#include "OSQP-Eigen/sr_solver.h"
#include "model/step_response_model.h"
#include "model/FSRModel.h"

#include <map>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include <Eigen/Dense>
using json = nlohmann::json; 
using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

class LightWeightMPC {
private:
    string sce_;
    int T_;
public:
    LightWeightMPC(const string& sce, int T);
};

#endif // LIGHTWEIGHT_MPC_H