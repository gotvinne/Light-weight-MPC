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

#include <map>

#include "IO/data_objects.h"
#include "IO/update.h"
#include "IO/parse.h"

void update_reference(json& arr, double value, int T) {
    for (int i = 0; i < T; i++) {
        arr.push_back(value);
    }
}

void update_system(string& sys_path, double ref, int T) {
    json sys_data = ReadJson(sys_path);

    // System variables
    std::map<string, int> m_map;
    CVData sd;
    MVData id;

    // Fetching data
    ParseSystemData(sys_data, m_map, sd, id, T);
}