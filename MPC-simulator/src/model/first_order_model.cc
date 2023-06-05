/**
 * @file first_order_model.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */
#include "model/first_order_model.h"
#include <cmath>

double StepResponse(double k, double tau, double theta, double dt, int i) {
    return k * (1 - exp(-(i * dt - theta) / tau));
}

std::vector<double> StepCoefficients(double k, double tau, double theta, int N) {
    double dt = (SETTLING_COEFFICIENT * tau + theta) / N;

    std::vector<double> step_coefficients_vec;
    step_coefficients_vec.resize(N); // To optimize memory consumption

    for (int i = 0; i < N; i++) {
        if ((i * dt) <= theta) {
            step_coefficients_vec[i] = 0;
        } else {
            step_coefficients_vec[i] = StepResponse(k, tau, theta, dt, i);
        }
    } 
    return step_coefficients_vec;
}
