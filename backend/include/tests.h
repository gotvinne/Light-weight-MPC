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
 * @param sce [std::string] Scenario name
 * @param sys [std::string] System name
 * @param SCE_PATH [std::string] Path to scenario folder, write folder
 * @param SYS_PATH [std::string] Path to system folder, read folder
 */
void TestSerializeScenario(const string& sce, const string& sys, const string& SCE_PATH, const string& SYS_PATH);

/**
 * @brief Test Wasm Simulate function
 * 
 */
void TestSimulate();

#endif // TESTS_H