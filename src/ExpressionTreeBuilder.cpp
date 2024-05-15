#include "ExpressionTreeBuilder.h"

ExpressionTreeBuilder::ExpressionTreeBuilder()
{
    _lastAddedNode = NULL;
}

void ExpressionTreeBuilder::insertOperand(std::shared_ptr<ExpressionTreeNode> node)
{
    if (_lastAddedNode != NULL && _lastAddedNode->getNodeType() != NodeType::Operator)
    {
        throw SyntaxError("Operator must precede operand");
    }

    if (_lastAddedNode != NULL) 
    {
        auto lastOperatorNode = std::dynamic_pointer_cast<OperatorNode>(_lastAddedNode);
        lastOperatorNode->rightOperand = node;
        node->parent = lastOperatorNode;
    }

    _lastAddedNode = node;
}

void ExpressionTreeBuilder::insertOperator(std::shared_ptr<OperatorNode> node)
{
    if (_lastAddedNode == NULL) 
    {
        throw SyntaxError("Cannot start expression with operator");
    }
    else if (_lastAddedNode->getNodeType() == NodeType::Operator)
    {
        throw SyntaxError("Operand must precede operator");
    }

    if (_lastAddedNode->parent == NULL)
    {
        _lastAddedNode->parent = node;
        node->leftOperand = _lastAddedNode;
    }
    else 
    {
        auto newNodeParent = std::dynamic_pointer_cast<OperatorNode>(_lastAddedNode->parent);
        while (newNodeParent->parent != NULL && newNodeParent->precidence >= node->precidence) 
        {
            newNodeParent = std::dynamic_pointer_cast<OperatorNode>(newNodeParent);
        }

        if (newNodeParent->parent == NULL && newNodeParent->precidence >= node->precidence)
        {
            node->leftOperand = newNodeParent;
            newNodeParent->parent = node;
        }
        else
        {
            node->leftOperand = newNodeParent->rightOperand;
            node->parent = newNodeParent;
            newNodeParent->rightOperand = node;
        }
    }
    _lastAddedNode = node;
}

std::shared_ptr<ExpressionTreeNode> ExpressionTreeBuilder::getRoot()
{
    return _lastAddedNode->root();
}
