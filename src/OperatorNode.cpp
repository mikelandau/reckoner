#include "OperatorNode.h"

double OperatorNode::calculate()
{
    return operation(leftOperand->calculate(), rightOperand->calculate());
}

NodeType OperatorNode::getNodeType()
{
    return NodeType::Operator;
}

std::string OperatorNode::serializeToJson(int level)
{
    std::string tabs = std::string(level, '\t');
    std::stringstream ss;

    ss << "{" << std::endl;
    ss << tabs << "\"type\": \"Operator\"," << std::endl;
    ss << tabs << "\"value\": \"" << displayName << "\"" << std::endl;
    ss << tabs << "\"leftOperand\": " << leftOperand->serializeToJson(level + 1) << "," << std::endl;
    ss << tabs << "\"rightOperand\": " << rightOperand->serializeToJson(level + 1) << std::endl;
    ss << tabs << "}";

    return ss.str();

}
