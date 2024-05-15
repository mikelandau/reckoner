#include "TokenType.h"

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
        case TokenType::NumericValue:
            return "NumericValue";
        case TokenType::OperatorAdd:
            return "OperatorAdd";
        case TokenType::OperatorDivide:
            return "OperatorDivide";
        case TokenType::OperatorMultiply:
            return "OperatorMultiply";
        case TokenType::OperatorPower:
            return "OperatorPower";
        case TokenType::OperatorSubtract:
            return "OperatorSubtract";
        case TokenType::ParentheticalTerm:
            return "ParentheticalTerm";
        case TokenType::Undefined:
            return "Undefined";
        default:
            return "?Unknown?";
    }
}