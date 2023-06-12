/**
 * @file json_specifiers.h
 * @author Geir Ola Tvinnereim
 * @copyright  Released under the terms of the BSD 3-Clause License
 * @date 2022
 */
#ifndef JSON_SPECIFIERS_H
#define JSON_SPECIFIERS_H

#include <string>
using string = std::string;

// ------- System file specifiers ------- //
const string kModel = "model";
const string kN_CV = "n_CV";
const string kN_MV = "n_MV";
const string kN = "N";

const string kCV = "CV";
const string kMV = "MV";
const string kOutput = "output";
const string kInput = "input";
const string kInit = "init";
const string kUnit = "unit";
const string kS = "S"; 

// ------- Scenario file specifiers ------- //
const string kSystem = "system";
const string kMPC = "MPC";
const string kP = "P";
const string kM = "M";
const string kW = "W";
const string kQ = "Q";
const string kR = "R";
const string kRoH = "RoH";
const string kRoL = "RoL";
const string kC = "c"; 
const string kDu = "du";

// ------- Simulation file specifiers ------- //
const string kScenario = "scenario";
const string kT = "T";
const string kDuTilde = "du_tilde";

const string kY = "y";
const string kY_pred = "y_pred";
const string kU = "u";
const string kRef = "ref"; 

#endif // JSON_SPECIFIERS_H
