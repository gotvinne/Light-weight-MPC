/**
 * @file plant_model.h
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */

#ifndef PLANT_MODEL_H
#define PLANT_MODEL_H

static const float WHITE_NOISE_MEAN = 0.0;

/**
 * @brief Calculate white noise based on a random generator
 * 
 * @param sigma varience of the white noise
 * @return float 
 */
float WhiteNoise(float sigma);

// function solving reference model ODE, representing a sensor measurement

#endif  // PLANT_MODEL_H