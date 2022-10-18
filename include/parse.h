/**
 * @file parse.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

/** File parsing scenario and system files  */

#ifndef PARSE_H
#define PARSE_H

#include <nlohmann/json.hpp>
#include <string>
#include <tuple>
#include <array>

using json = nlohmann::json;

// system file specifiers
const int kModelParam = 3;

const std::string kModel = "model";
const std::string kN_CV = "n_CV";
const std::string kN_MV = "n_MV";
const std::string kN = "N";

// scenario file specifiers


//

json read_json(std::string filepath);
std::array<int,kModelParam> system_model_data(json data);

void print_container(std::array<int,3> container);



#endif  // PARSE_H