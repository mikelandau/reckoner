#include "ExpressionTreeNode.h"

std::shared_ptr<ExpressionTreeNode> ExpressionTreeNode::root()
{
    if (parent == NULL) return shared_from_this();
    auto root = parent;
    while (root->parent != NULL) {
        root = root->parent;
    }
    return root;
}