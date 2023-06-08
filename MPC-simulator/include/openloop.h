/**
 * @file openloop.h
 * @author Geir Ola Tvinnereim
 * @brief module implementing open loop simulation
 * @version 0.1
 * @date 2023-06-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef OPENLOOP_H
#define OPENLOOP_H

#include <string>
using string = std::string;

/**
 * @brief Open loop simulation of FSR model
 * 
 * @param sys System name
 * @param ref_str Reference values
 * @param ref_str Reference values
 * @param T Simulations steps
 */
void OpenLoopFSRM(const string& sys, const string& ref_str, const string& step_str, int T);

#endif // OPENLOOP_H