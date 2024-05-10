#pragma once

#include <memory>
#include <string>

enum class NodeType
{
    NumericValue,
    Operator
};

struct ExpressionTreeNode 
{
    std::shared_ptr<ExpressionTreeNode> parent;
    virtual double calculate() = 0;
    virtual NodeType getNodeType() = 0;
    virtual std::string serializeToJson(int level) = 0;
};
