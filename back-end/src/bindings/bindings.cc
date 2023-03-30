#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#endif // __EMSCRIPTEN__

#include "IO/serialize.h"
#include <stdlib.h>

#include <string>

std::string say_hello(std::string str) {
    return str;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("sayHello", &say_hello);
}
#endif // __EMSCRIPTEN__
