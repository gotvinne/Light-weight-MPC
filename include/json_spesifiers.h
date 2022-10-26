/**
 * @file data_constants.h
 * @author Geir Ola Tvinnereim
 * @copyright  Geir Ola Tvinnereim 
 * @date 2022
 */

#ifndef DATA_CONSTANTS_H
#define DATA_CONSTANTS_H

#include <string>

// ------- System file specifiers ------- //
const std::string kModel = "model";
const std::string kN_CV = "n_CV";
const std::string kN_MV = "n_MV";
const std::string kN = "N";

const std::string kCV = "CV";
const std::string kMV = "MV";
const std::string kState = "state";
const std::string kInput = "input";
const std::string kInit = "init";
const std::string kS = "S"; 
const std::string kU = "u";

// ------- Scenario file specifiers ------- //
const std::string kSystem = "system";

const std::string kMPC = "MPC";
const std::string kP = "P";
const std::string kM = "M";
const std::string kW = "W";
const std::string kQ = "Q";
const std::string kR = "R";
const std::string kRo = "Ro";
const std::string kBu = "bias update";

const std::string kC = "c_i"; 
// ------- ------- ------- ------- ------- //

#endif // DATA_CONSTANTS_H