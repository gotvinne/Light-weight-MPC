/**
 * @file plant_model.cc
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */

#include "model/plant_model.h"
#include <random>

float WhiteNoise(float sigma) {
    std::default_random_engine generator;
    std::normal_distribution<float> dist(WHITE_NOISE_MEAN, sigma);
    return dist(generator);
}