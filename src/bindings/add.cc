#include <emscripten/emscripten.h>
#include <stdlib.h>

extern "C" {
    EMSCRIPTEN_KEEPALIVE int add(int a, int b)
    {return a + b;}
}