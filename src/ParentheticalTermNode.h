#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "ExpressionTreeNode.h"

struct ParentheticalTermNode : ExpressionTreeNode 
{
    std::shared_ptr<ExpressionTreeNode> subtreeRoot;
    virtual double calculate();
    virtual NodeType getNodeType();
    virtual std::string serializeToJson(int level);
};