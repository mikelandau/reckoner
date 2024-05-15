#pragma once

#include <string>

enum class TokenType 
{
    NumericValue,
    OperatorAdd,
    OperatorDivide,
    OperatorMultiply,
    OperatorPower,
    OperatorSubtract,
    ParentheticalTerm,
    Undefined,
};

std::string tokenTypeToString(TokenType type);
