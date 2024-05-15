#pragma once

#include <memory>

#include "ExpressionTreeNode.h"
#include "NumericValueNode.h"
#include "OperatorNode.h"
#include "ParentheticalTermNode.h"
#include "SyntaxError.h"

class ExpressionTreeBuilder
{
    public:
        ExpressionTreeBuilder();
        void insertOperand(std::shared_ptr<ExpressionTreeNode> node);
        void insertOperator(std::shared_ptr<OperatorNode> node);
        std::shared_ptr<ExpressionTreeNode> getRoot();
    private:
        std::shared_ptr<ExpressionTreeNode> _lastAddedNode;
};