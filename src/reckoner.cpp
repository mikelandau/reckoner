#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#include <emscripten/emscripten.h>

#include "ExpressionTreeBuilder.h"
#include "ExpressionTreeNode.h"
#include "NumericValueNode.h"
#include "NodeCategory.h"
#include "OperatorNode.h"
#include "TokenType.h"

std::shared_ptr<ExpressionTreeNode>    buildExpressionTree(std::string input);
int                                    countCharactersInString (std::string s, char c);
std::shared_ptr<NumericValueNode>      createNumericValueNodeFromToken(std::string token);
std::shared_ptr<OperatorNode>          createOperatorNodeFromToken(std::string token, TokenType tokenType);
std::shared_ptr<ParentheticalTermNode> createParentheticalTermNodeFromToken(std::string token);
TokenType                              determineTokenType (std::string token, NodeCategory lastAddedNodeCategory);
bool                                   isOperatorType(TokenType type);
bool                                   isStringNumeric (std::string s);
extern "C" EMSCRIPTEN_KEEPALIVE double reckon(const char* cstr_input, char* tree_json);

std::shared_ptr<ExpressionTreeNode> buildExpressionTree(std::string input)
{
    if (input.empty())
    {
        throw SyntaxError("Expression cannot be empty");
    }
    int tokenStart = 0;
    NodeCategory lastAddedNodeCategory = NodeCategory::Operator;
    
    auto expressionTreeBuilder = std::make_unique<ExpressionTreeBuilder>();
   
    for (int i = 1; i + tokenStart <= input.length(); ++i)
    {
        std::string token = input.substr(tokenStart, i);
        TokenType tokenType = determineTokenType(token, lastAddedNodeCategory);

        std::cout << "building: " << token << std::endl;

        bool lastLoopIteration = i + tokenStart == input.length();

        if (lastLoopIteration && tokenType == TokenType::Undefined)
        {
            throw SyntaxError("Expression must end with valid operand");
        }
        
        TokenType lookAheadTokenType = TokenType::Undefined;
        if (!lastLoopIteration)
        {
            std::string lookAheadToken = input.substr(tokenStart, i + 1);
            lookAheadTokenType = determineTokenType(lookAheadToken, lastAddedNodeCategory);
        }

        if (tokenType != TokenType::Undefined && lookAheadTokenType == TokenType::Undefined)
        {
            std::cout << "found token: " << token << std::endl;
            if (tokenType == TokenType::NumericValue) 
            {
                auto numericValueNode = createNumericValueNodeFromToken(token);
                expressionTreeBuilder->insertOperand(numericValueNode);
                lastAddedNodeCategory = NodeCategory::Operand;
            }
            else if (tokenType == TokenType::ParentheticalTerm)
            {
                auto parentheticalTermNode = createParentheticalTermNodeFromToken(token);
                expressionTreeBuilder->insertOperand(parentheticalTermNode);
                lastAddedNodeCategory = NodeCategory::Operand;
            }
            else if (isOperatorType(tokenType))
            {
                auto operatorNode = createOperatorNodeFromToken(token, tokenType);
                expressionTreeBuilder->insertOperator(operatorNode);
                lastAddedNodeCategory = NodeCategory::Operator;
            }
            else
            {
                throw std::runtime_error("unsupported token type found");
            }

            tokenStart += i;
            i = 0;
        }
    }

    return expressionTreeBuilder->getRoot();
}

int countCharactersInString (std::string s, char c)
{
    std::string::difference_type n = std::count(s.begin(), s.end(), c);
    return (int)n;
}

std::shared_ptr<NumericValueNode> createNumericValueNodeFromToken(std::string token)
{
    auto numericValueNode = std::make_shared<NumericValueNode>();
    numericValueNode->value = std::stod(token);
    return numericValueNode;
}

std::shared_ptr<OperatorNode> createOperatorNodeFromToken(std::string token, TokenType tokenType)
{
    auto operatorNode = std::make_shared<OperatorNode>();
    operatorNode->displayName = token;
    switch (tokenType)
    {
        case TokenType::OperatorPower:
            operatorNode->operation = [](double a, double b) { return pow(a, b); };
            operatorNode->precidence = 30;
            break;
        case TokenType::OperatorMultiply:
            operatorNode->operation = [](double a, double b) { return a * b; };
            operatorNode->precidence = 20;
            break;
        case TokenType::OperatorDivide:
            operatorNode->operation = [](double a, double b) { return a / b; };
            operatorNode->precidence = 20;
            break;
        case TokenType::OperatorAdd:
            operatorNode->operation = [](double a, double b) { return a + b; };
            operatorNode->precidence = 10;
            break;
        case TokenType::OperatorSubtract:
            operatorNode->operation = [](double a, double b) { return a - b; };
            operatorNode->precidence = 10;
            break;
        default:
            throw std::invalid_argument("invalid tokenType for operatorNode");
    }
    return operatorNode;
}

std::shared_ptr<ParentheticalTermNode> createParentheticalTermNodeFromToken(std::string token)
{
    auto subTerm = token.substr(1, token.length() - 2);
    auto parentheticalTermNode = std::make_shared<ParentheticalTermNode>();
    auto subTree = buildExpressionTree(subTerm);
    parentheticalTermNode->subtreeRoot = subTree;
    return parentheticalTermNode;
}

TokenType determineTokenType (std::string token, NodeCategory lastAddedNodeCategory)
{
    if (lastAddedNodeCategory == NodeCategory::Operand)
    {
        if (token == "^")
        {
            return TokenType::OperatorPower;
        }
        else if (token == "*") 
        {
            return TokenType::OperatorMultiply;
        }
        else if (token == "/")
        {
            return TokenType::OperatorDivide;
        }
        else if (token == "+")
        {
            return TokenType::OperatorAdd;
        }
        else if (token == "-")
        {
            return TokenType::OperatorSubtract;
        }
    }
    else 
    {
        auto openParens = countCharactersInString(token, '(');
        auto closeParens = countCharactersInString(token, ')');
        auto firstChar = token.at(0);

        if (token.at(0) == '(' && token.at(token.length() - 1) == ')' && countCharactersInString(token, '(') == countCharactersInString(token, ')')) {
            return TokenType::ParentheticalTerm;
        }
        else if (isStringNumeric(token)) 
        {
            return TokenType::NumericValue;
        }
    }
    return TokenType::Undefined;
}

bool isOperatorType(TokenType type)
{
    return type == TokenType::OperatorAdd ||
        type == TokenType::OperatorDivide ||
        type == TokenType::OperatorMultiply ||
        type == TokenType::OperatorPower ||
        type == TokenType::OperatorSubtract;
}

bool isStringNumeric (std::string s)
{
    if (s.empty() || s == "-") 
    {
        return false;
    }

    int dotCount = 0;

    for (int i = 0; i < s.length(); ++i)
    {
        char c = s.at(i);
        if (i == 0 && c == '-') {
            continue;
        }

        if (c == '.' && dotCount == 0) 
        {
            ++dotCount;
        }
        else if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

int main() 
{
    std::cout << "Ready to Reckon!" << std::endl;
    return 0;
}

extern "C" EMSCRIPTEN_KEEPALIVE double reckon(const char* cstr_input, char* tree_json) 
{
    std::string input (cstr_input);
    input.erase(remove_if(input.begin(), input.end(), isspace), input.end());

    std::shared_ptr<ExpressionTreeNode> expressionTree;

    expressionTree = buildExpressionTree(input);
    
    auto result = expressionTree->calculate();

    auto tree = expressionTree->serializeToJson(0);

    strncpy(tree_json, tree.c_str(), 10000); // have faith

    return result;
}
