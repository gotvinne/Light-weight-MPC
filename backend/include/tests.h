/**
 * @file tests.h
 * @author Geir Ola Tvinnereim
 * @brief Scripts defining tests for TDD
 * @version 0.1
 * @date 2023-05-03
 * 
 * @copyright Released under the terms of the BSD 3-Clause License
 * 
 */
#ifndef TESTS_H
#define TESTS_H

#include <string>
using string = std::string;

/**
 * @brief Test ScerializeScenario functionality
 * 
 * @param sce Scenario name
 * @param sys System name
 * @param SCE_PATH Path to scenario folder, write folder
 * @param SYS_PATH Path to system folder, read folder
 */
void TestSerializeScenario(const string& sce, const string& sys, const string& SCE_PATH, const string& SYS_PATH);

/**
 * @brief Test Wasm Simulate function, simulate scenario sce_sys.json
 * 
 * @param sys System name
 * @param ref_vec vector of references, must concide with system
 * @param T MPC horizon
 */
void TestSimulate(const string& sys, const string& ref_vec, int T);

#endif // TESTS_H