/**
 * @file step_coefficients.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef STEP_COEFFICIENTS_H
#define STEP_COEFFICIENTS_H

#include <vector>

/* Module creating finite step response model for a first order system */

const int SETTLING_COEFFICIENT = 5;

/**
 * @brief Calculating step response for a first order system given parameters.
 * 
 * @param k transfer function gain
 * @param tau time constant
 * @param theta time delay
 * @param dt 
 * @param i 
 * @return float 
 */
float step_response(float k, float tau, float theta, float dt, int i);

/**
 * @brief Calculating step response coefficients given system and horizon.
 * 
 * @param k transfer function gain
 * @param tau time constant
 * @param theta time delay
 * @param N Number of step coefficients
 * @return std::vector<float> holding the step coefficients for the given parameters
 */
std::vector<float> step_coefficients(float k, float tau, float theta, int N);

void print_coefficients(const std::vector<float> &vec);

#endif  // STEP_COEFFICIENTS_H
