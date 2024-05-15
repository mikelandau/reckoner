#pragma once

#include <memory>
#include <string>

enum class NodeType
{
    NumericValue,
    Operator,
    ParentheticalTerm
};

struct ExpressionTreeNode : public std::enable_shared_from_this<ExpressionTreeNode>
{
    std::shared_ptr<ExpressionTreeNode> parent;
    std::shared_ptr<ExpressionTreeNode> root();
    virtual double calculate() = 0;
    virtual NodeType getNodeType() = 0;
    virtual std::string serializeToJson(int level) = 0;
};
