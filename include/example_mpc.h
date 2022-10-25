// Copyright (C) Geir Ola Tvinnereim 2022 

#ifndef EXAMPLE_MPC_H
#define EXAMPLE_MPC_H

/**
 * @file MPCExample.cpp
 * @author Giulio Romualdi
 * @copyright Released under the terms of the BSD 3-Clause License
 * @date 2018
 */


// osqp-eigen
#include "OsqpEigen/OsqpEigen.h"

// eigen
#include <Eigen/Dense>
#include <iostream>

void setDynamicsMatrices(Eigen::Matrix<double, 12, 12> &a, Eigen::Matrix<double, 12, 4> &b);

void setInequalityConstraints(Eigen::Matrix<double, 12, 1> &xMax, Eigen::Matrix<double, 12, 1> &xMin,
                              Eigen::Matrix<double, 4, 1> &uMax, Eigen::Matrix<double, 4, 1> &uMin);

void setWeightMatrices(Eigen::DiagonalMatrix<double, 12> &Q, Eigen::DiagonalMatrix<double, 4> &R);

void castMPCToQPHessian(const Eigen::DiagonalMatrix<double, 12> &Q, const Eigen::DiagonalMatrix<double, 4> &R, int mpcWindow,
                        Eigen::SparseMatrix<double> &hessianMatrix);

void castMPCToQPGradient(const Eigen::DiagonalMatrix<double, 12> &Q, const Eigen::Matrix<double, 12, 1> &xRef, int mpcWindow,
                         Eigen::VectorXd &gradient);

void castMPCToQPConstraintMatrix(const Eigen::Matrix<double, 12, 12> &dynamicMatrix, const Eigen::Matrix<double, 12, 4> &controlMatrix,
                                 int mpcWindow, Eigen::SparseMatrix<double> &constraintMatrix);

void castMPCToQPConstraintVectors(const Eigen::Matrix<double, 12, 1> &xMax, const Eigen::Matrix<double, 12, 1> &xMin,
                                   const Eigen::Matrix<double, 4, 1> &uMax, const Eigen::Matrix<double, 4, 1> &uMin,
                                   const Eigen::Matrix<double, 12, 1> &x0,
                                   int mpcWindow, Eigen::VectorXd &lowerBound, Eigen::VectorXd &upperBound);


void updateConstraintVectors(const Eigen::Matrix<double, 12, 1> &x0,
                             Eigen::VectorXd &lowerBound, Eigen::VectorXd &upperBound);

double getErrorNorm(const Eigen::Matrix<double, 12, 1> &x,
                    const Eigen::Matrix<double, 12, 1> &xRef);

int solver();


#endif  // EXAMPLE_MPC_H