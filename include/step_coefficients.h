// Copyright (C) Geir Ola Tvinnereim 2022 

#ifndef STEP_COEFFICIENTS_H
#define STEP_COEFFICIENTS_H

#include <vector>

const int SETTLING_COEFFICIENT = 5;
float step_response(float k, float tau, float theta, float dt, int i);
std::vector<float> step_coefficients(float k, float tau, float theta, int N);
void print_coefficients(const std::vector<float> &vec);

#endif  // STEP_COEFFICIENTS_H