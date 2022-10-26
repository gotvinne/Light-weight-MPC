/**
 * @file sr_solver.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */
#include "sr_solver.h"
#include "json_specifiers.h"

#include "OsqpEigen/OsqpEigen.h"
#include <Eigen/Dense>

#include <map>
#include <string>


void sr_solver(const int& T, const std::map<std::string,int> model_param) {

    OsqpEigen::Solver solver;
    solver.settings()->setWarmStart(true); // Starts primal and dual variables from previous QP

    // Define QP
    solver.data()->setNumberOfVariables(model_param)

    
    //int n = 

    // For diagonal matrices
    /*
    Eigen::Vector4f diag_Vec(1, 2, 4, 7);
    Eigen::Matrix4f Mat = diag_Vec.matrix().asDiagonal(); // Transfer vector to diagonal matrix.*/
}