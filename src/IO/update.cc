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

#include <exception>

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

void UpdateReference(const std::string& sys, std::vector<double>& vec, int T) {
    const string sys_path = "../data/systems/"+ sys + ".json";
    json sys_data = ReadJson(sys_path);
    
    if (sys_data.at(kCV).size() != vec.size()) {
        throw std::runtime_error("Not enough references to update system");
    }

    int cv = 0;
    for (auto& elem : sys_data.at(kCV)) {
        json new_ref = json::array();
        getReference(new_ref, vec[cv], T);

        elem[kY_Ref] = new_ref; // Set new reference
        cv++;
    }

    WriteJson(sys_data, sys_path);
}