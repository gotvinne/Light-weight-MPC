#include <emscripten/emscripten.h>
#include <stdlib.h>

#include "IO/serialize.h"


extern "C" {
    EMSCRIPTEN_KEEPALIVE 
    int add(int a, int b){return a + b;}

    char* testChar(char* str) { return str; }
}