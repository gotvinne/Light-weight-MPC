/**
 * @file first_order_model.h
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */
#ifndef FISRT_ORDER_MODEL_H
#define FISRT_ORDER_MODEL_H

#include <vector>
/* Module creating finite step response model for a first order system */

static const int SETTLING_COEFFICIENT = 5;

/**
 * @brief Calculating step response for a first order system given parameters.
 * 
 * @param k transfer function gain
 * @param tau time constant
 * @param theta time delay
 * @param dt time step
 * @param i index
 * @return double 
 */
double StepResponse(double k, double tau, double theta, double dt, int i);

/**
 * @brief Calculating step response coefficients given system and horizon.
 * 
 * @param k transfer function gain
 * @param tau time constant
 * @param theta time delay
 * @param N Number of step coefficients
 * @return std::vector<double> holding the step coefficients for the given parameters
 */
std::vector<double> StepCoefficients(double k, double tau, double theta, int N);

#endif  // FISRT_ORDER_MODEL_H
