#include "Operation.h"

Operation::Operation(std::regex expression, double(*func)(double, double)) 
{
    _expression = expression;
    _func = func;
}

double Operation::execute(double a, double b) 
{
    return _func(a, b);
}

std::regex const Operation::getExpression()
{
    return _expression;
}
