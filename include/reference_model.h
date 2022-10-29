/**
 * @file reference_model.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef REFERENCE_MODEL_H
#define REFERENCE_MODEL_H

const float WHITE_NOISE_MEAN = 0.0;

float WhiteNoise(float sigma);

// function solving reference model ODE, representing a sensor measurement

#endif  // REFERENCE_MODEL_H