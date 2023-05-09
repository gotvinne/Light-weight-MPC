/**
 * @file step_response_model.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */
#include "model/step_response_model.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <string> 

float StepResponse(float k, float tau, float theta, float dt, int i) {
    return k * (1 - exp(-(i * dt - theta) / tau));
}

std::vector<float> StepCoefficients(float k, float tau, float theta, int N) {
    float dt = (SETTLING_COEFFICIENT * tau + theta) / N;

    std::vector<float> step_coefficients_vec;
    step_coefficients_vec.resize(N);

    for (int i = 0; i < N; i++) {
        if ((i * dt) <= theta) {
            step_coefficients_vec[i] = 0;
        } else {
            step_coefficients_vec[i] = StepResponse(k, tau, theta, dt, i);
        }
    } 
    return step_coefficients_vec;
}
