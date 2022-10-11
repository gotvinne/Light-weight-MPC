// Copyright (C) Geir Ola Tvinnereim 2022 

#ifndef FORMATTING_H
#define FORMATTING_H 

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

/**
 * @brief 
 * 
 * @param filepath 
 * @return json 
 */
json parse(std::string filepath);

#endif  // FORMATTING_H