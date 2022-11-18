/**
 * @file step_response_model.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef STEP_RESPONSE_MODEL_H
#define STEP_RESPONSE_MODEL_H

#include <vector>
#include "Eigen/Dense"
#include "parse.h"

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
float StepResponse(float k, float tau, float theta, float dt, int i);

/**
 * @brief Calculating step response coefficients given system and horizon.
 * 
 * @param k transfer function gain
 * @param tau time constant
 * @param theta time delay
 * @param N Number of step coefficients
 * @return std::vector<float> holding the step coefficients for the given parameters
 */
std::vector<float> StepCoefficients(float k, float tau, float theta, int N);

void PrintCoefficients(const std::vector<float> &vec);

#endif  // STEP_RESPONSE_MODEL_H
