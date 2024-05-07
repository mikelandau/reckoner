#pragma once

#include <regex>

class Operation 
{
    public:
        Operation(std::regex expression, double(*func)(double, double));
        double execute(double a, double b);
        std::regex const getExpression();
    private:
        std::regex _expression;
        double (*_func)(double, double);
};
