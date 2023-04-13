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

std::string validateSystem(std::string system) {
    std::string filepath = system + ".json";
    std::ifstream system_file(filepath);
    if (system_file.good()) {
        json sys_data = json::parse(system_file);

        json obj;
        obj[kN_CV] = sys_data.at(kModel).at(kN_CV);
        obj[kN_MV] = sys_data.at(kModel).at(kN_MV);
        obj[kN] = sys_data.at(kModel).at(kN);

        return to_string(obj);  // Return parsed object as string
    }
    else {
        return "Invalid system";
    }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("validateSystem", &validateSystem);
    emscripten::function("sayHello", &say_hello);
}
#endif // __EMSCRIPTEN__
