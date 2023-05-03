/**
 * @file wasm.h
 * @author Geir Ola Tvinnereim
 * @brief Defining Emscripten includes
 * @version 0.1
 * @date 2023-05-02
 * 
 * @copyright Released under the terms of the BSD 3-Clause License
 * 
 */

#ifndef WASM_H
#define WASM_H

#include "IO/data_objects.h"
#include "IO/json_specifiers.h"
#include "IO/parse.h"
#include "IO/serialize.h"

#include "MPC/solvers.h"
#include "model/FSRModel.h"

#include <string>
#include <map>
#include <vector>
#include <Eigen/Dense>

using VectorXd = Eigen::VectorXd;
using MatrixXd = Eigen::MatrixXd;
using string = std::string;

#endif // WASM_H