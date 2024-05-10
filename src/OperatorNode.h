#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "ExpressionTreeNode.h"

struct OperatorNode : ExpressionTreeNode
{
    std::string displayName;
    int precidence;
    std::shared_ptr<ExpressionTreeNode> leftOperand;
    std::shared_ptr<ExpressionTreeNode> rightOperand;
    double (*operation)(double, double);
    virtual double calculate();
    virtual NodeType getNodeType();
    virtual std::string serializeToJson(int level);
};

