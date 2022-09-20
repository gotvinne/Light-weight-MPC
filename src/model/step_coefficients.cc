// Copyright (C) Geir Ola Tvinnereim 2022

#include <vector>
#include <cmath>
#include "step_coefficients.h"

float step_response(float k, float tau, float theta, float dt, int i) {
    return k * (1 - exp(-(i * dt - theta) / tau));
}

std::vector<float> step_coefficients(float k, float tau, float theta, int N) {
    float dt = (SETTLING_COEFFICIENT * tau + theta) / N;

    std::vector<float> step_coefficients_vec;
    step_coefficients_vec.reserve(N);

    for (int i = 0; i < N; i++) {
        if ((i * dt) <= theta) {
            step_coefficients_vec[i] = 0;
        } else {
            step_coefficients_vec[i] = step_response(k, tau, theta, dt, i);
        }
    } 
    return step_coefficients_vec;
}

