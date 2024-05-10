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
    ss << tabs << "\t\"type\": \"Operator\"," << std::endl;
    ss << tabs << "\t\"value\": \"" << displayName << "\"" << std::endl;
    ss << tabs << "\t\"leftOperand\": " << leftOperand->serializeToJson(level + 1) << "," << std::endl;
    ss << tabs << "\t\"rightOperand\": " << rightOperand->serializeToJson(level + 1) << std::endl;
    ss << tabs << "}";

    return ss.str();

}
