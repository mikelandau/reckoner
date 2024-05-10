#pragma once

#include <sstream>
#include <string>

#include "ExpressionTreeNode.h"

struct NumericValueNode : ExpressionTreeNode
{
    double value;
    virtual double calculate();
    virtual NodeType getNodeType();
    virtual std::string serializeToJson(int level);
};

