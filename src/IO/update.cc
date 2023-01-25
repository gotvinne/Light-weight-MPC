/**
 * @file update.cc
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "IO/update.h"
#include "IO/parse.h"
#include "IO/serialize.h"
#include "IO/json_specifiers.h"

#include <iostream>

/**
 * @brief Get the Reference object
 * 
 * @param arr 
 * @param value 
 * @param T 
 */
static void getReference(json& arr, double value, int T) {
    for (int i = 0; i < T; i++) {
        arr.push_back(value);
    }
}

void UpdateReference(const std::string& sys_path, double ref, int T) {
    json sys_data = ReadJson(sys_path);
    json arr = json::array();

    getReference(arr, ref, T);
    for (auto& elem : sys_data.at(kCV)) {
        std::cout << elem[kOutput] << std::endl; 
        elem[kY_Ref] = arr;
    }

    WriteJson(sys_data, sys_path);
}