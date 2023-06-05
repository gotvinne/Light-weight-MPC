/**
 * @file wasm.cc
 * @author Geir Ola Tvinnereim
 * @brief Emscripten module exporting functions to mjs
 * @version 0.1
 * @date 2023-04-18
 * 
 * @copyright  Released under the terms of the BSD 3-Clause License
 * 
 */
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

#include "wasm/wasm.h"

// Functions added into EMCRIPTEN_BINDINGS are compiled to mpc_simulator.mjs using Webassembly.
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("simulate", &simulate);
}
#endif // __EMSCRIPTEN__
