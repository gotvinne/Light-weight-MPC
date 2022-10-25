/**
 * @file step_coefficients.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef STEP_COEFFICIENTS_H
#define STEP_COEFFICIENTS_H

#include <vector>

const int SETTLING_COEFFICIENT = 5;

/**
 * @brief 
 * 
 * @param k 
 * @param tau 
 * @param theta 
 * @param dt 
 * @param i 
 * @return float 
 */
float step_response(float k, float tau, float theta, float dt, int i);

/**
 * @brief 
 * 
 * @param k 
 * @param tau 
 * @param theta 
 * @param N 
 * @return std::vector<float> 
 */
std::vector<float> step_coefficients(float k, float tau, float theta, int N);

/**
 * @brief 
 * 
 * @param vec 
 */
void print_coefficients(const std::vector<float> &vec);

#endif  // STEP_COEFFICIENTS_H