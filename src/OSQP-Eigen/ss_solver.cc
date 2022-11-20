/**
 * @file ss_solver.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */
#include "OSQP-Eigen/ss_solver.h"

#include "OsqpEigen/OsqpEigen.h"
#include <Eigen/Dense>

void ss_solver() {

    OsqpEigen::Solver solver;
    int sim_steps; 
    
    //int n = 

    // For diagonal matrices
    /*
    Eigen::Vector4f diag_Vec(1, 2, 4, 7);
    Eigen::Matrix4f Mat = diag_Vec.matrix().asDiagonal(); // Transfer vector to diagonal matrix.*/
}