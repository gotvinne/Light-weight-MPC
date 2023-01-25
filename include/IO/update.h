/**
 * @file update.h
 * @author Geir Ola Tvinnereim
 * @brief Functionality to update a scenario or simulation file for further simulation.
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef UPDATE_H
#define UPDATE_H

#include <string>

#include "nlohmann/json"


using json = nhlomann::json;

void update_reference(json& arr, double value, int T);

void update_system(string sys_path, double ref, int T);


#endif // UPDATE_H