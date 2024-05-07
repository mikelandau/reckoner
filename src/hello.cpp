#include <iostream>
#include <string>

#include <emscripten/emscripten.h>

int main() 
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

extern "C" EMSCRIPTEN_KEEPALIVE double reckon(const char* cstr_input) 
{
    std::string input (cstr_input);
    return 666.0;
}
