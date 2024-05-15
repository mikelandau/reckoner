#pragma once

#include <stdexcept>
#include <string>

class SyntaxError : public std::runtime_error
{
    public:
        SyntaxError(const std::string& what) : std::runtime_error(what) { }
};
