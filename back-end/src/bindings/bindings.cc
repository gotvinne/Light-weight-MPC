/**
 * @file bindings.cc
 * @author Geir Ola Tvinnereim
 * @brief 
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright  Released under the terms of the BSD 3-Clause License
 * 
 */

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#endif // __EMSCRIPTEN__

#include "IO/json_specifiers.h"
#include <stdlib.h>

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string say_hello(std::string str) {
    return str;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("sayHello", &say_hello);
}
#endif // __EMSCRIPTEN__
