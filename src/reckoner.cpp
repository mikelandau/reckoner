#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <emscripten/emscripten.h>

#include "ExpressionTreeNode.h"
#include "NumericValueNode.h"
#include "OperatorNode.h"

enum class Token 
{
    Operand,
    Operator
};

int main() 
{
    std::cout << "Ready to Reckon!" << std::endl;
    return 0;
}

std::shared_ptr<ExpressionTreeNode> getRoot(std::shared_ptr<ExpressionTreeNode> node)
{
    std::shared_ptr root = node;
    while(root->parent != NULL)
        root = root->parent;
    return root;
}

std::shared_ptr<OperatorNode> createOperatorNode(std::string operatorString) {
    auto node = std::make_shared<OperatorNode>();
    node->displayName = operatorString;
    if (operatorString == "+") 
    {
        node->operation = [](double a, double b) { return a + b; };
        node->precidence = 10;
    }
    else if (operatorString == "-") 
    {
        node->operation = [](double a, double b) { return a - b; };
        node->precidence = 10;
    }
    else if (operatorString == "*") 
    {
        node->operation = [](double a, double b) { return a * b; };
        node->precidence = 20;
    }
    else if (operatorString == "/") 
    {
        node->operation = [](double a, double b) { return a / b; };
        node->precidence = 20;
    }
    else 
    {
        throw std::invalid_argument("invalid operator");
    }
    return node;
}

std::shared_ptr<ExpressionTreeNode> makeExpressionTree(std::string input) 
{
    auto currentTokenType = Token::Operand;

    std::stringstream currentTokenBuilder;

    std::shared_ptr<ExpressionTreeNode> currentNode = NULL;

    std::cout << "Making tree for: " << input << std::endl;

    bool builderEmpty = true;

    for (int i = 0; i < input.length(); ++i) 
    {
        if (builderEmpty && currentTokenType == Token::Operand && input[i] == '(') 
        {
            int openParen = i;
            std::cout << "open: " << openParen << std::endl;
            // skip open parenthesis
            ++i;
            // skip over other characters within parentheses, parse it recursively
            for (int nestingLevel = 1; nestingLevel > 0; ++i) 
            {
                if (input[i] == '(') ++nestingLevel;
                if (input[i] == ')') --nestingLevel;
            }
            int closeParen = i;
            std::cout << "close: " << closeParen;

            std::string parenTerm = input.substr(openParen + 1, closeParen - openParen - 2);
            auto parenTree = makeExpressionTree(parenTerm);
            if (currentNode != NULL)
            {
                auto operatorNode = std::dynamic_pointer_cast<OperatorNode>(currentNode);
                parenTree->parent = operatorNode;
                operatorNode->rightOperand = parenTree;
            }
            currentNode = parenTree;
            currentTokenType = Token::Operator;
            if (i == input.length())
                break;
        }
        else if (currentTokenType == Token::Operand && !isdigit(input[i]) && !(input[i] == '.')) 
        {
            auto numericNode = std::make_shared<NumericValueNode>();
            auto currentToken = currentTokenBuilder.str();
            numericNode->value = stod(currentToken);
            currentTokenBuilder.str("");
            currentTokenBuilder.clear();
            builderEmpty = true;

            if (currentNode != NULL)
            {
                auto operatorNode = std::dynamic_pointer_cast<OperatorNode>(currentNode);
                numericNode->parent = operatorNode;
                operatorNode->rightOperand = numericNode;
            }
            currentNode = numericNode;
            currentTokenType = Token::Operator;
        }
        else if (currentTokenType == Token::Operator && (isdigit(input[i]) || input[i] == '.'))
        {
            auto currentToken = currentTokenBuilder.str();
            auto operatorNode = createOperatorNode(currentToken);
            currentTokenBuilder.str("");
            currentTokenBuilder.clear();
            builderEmpty = true;
            
            if (currentNode->parent == NULL)
            {
                currentNode->parent = operatorNode;
                operatorNode->leftOperand = currentNode;
            }
            else 
            {
                auto newParent = std::dynamic_pointer_cast<OperatorNode>(currentNode->parent);
                while (newParent->parent != NULL && newParent->precidence >= operatorNode->precidence) 
                {
                    newParent = std::dynamic_pointer_cast<OperatorNode>(newParent->parent);
                }
                if (newParent->parent == NULL && newParent->precidence >= operatorNode->precidence) 
                {
                    operatorNode->leftOperand = newParent;
                } 
                else 
                {
                    operatorNode->leftOperand = newParent->rightOperand;
                    operatorNode->parent = newParent;
                    newParent->rightOperand = operatorNode;
                }

            }
            currentNode = operatorNode;
            currentTokenType = Token::Operand;
        }
        currentTokenBuilder << input[i];
        builderEmpty = false;
    }
    auto numericNode = std::make_shared<NumericValueNode>();
    numericNode->value = stod(currentTokenBuilder.str());
    if (currentNode != NULL)
    {
        auto operatorNode = std::dynamic_pointer_cast<OperatorNode>(currentNode);
        numericNode->parent = operatorNode;
        operatorNode->rightOperand = numericNode;
    }
    currentNode = numericNode;
    currentTokenType = Token::Operator;

    auto root = getRoot(currentNode);

    return root;
}

extern "C" EMSCRIPTEN_KEEPALIVE double reckon(const char* cstr_input, char* tree_json) 
{
    std::string input (cstr_input);
    input.erase(remove_if(input.begin(), input.end(), isspace), input.end());

    std::shared_ptr<ExpressionTreeNode> expressionTree;

    expressionTree = makeExpressionTree(input);
    
    auto result = expressionTree->calculate();

    auto tree = expressionTree->serializeToJson(0);

    strncpy(tree_json, tree.c_str(), 10000); // have faith

    return result;
}
