#include <emscripten/emscripten.h>
#include <stdlib.h>

#include "IO/serialize.h"

#include <string>


const int = 0;
extern "C" {
    EMSCRIPTEN_KEEPALIVE 
    int add(int a, int b){return a + b;}


}