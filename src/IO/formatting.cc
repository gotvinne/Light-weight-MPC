// Copyright (C) Geir Ola Tvinnereim 2022

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

json parse(std::string filepath) {
    std::ifstream file(filepath);
    return json::parse(file);
}
