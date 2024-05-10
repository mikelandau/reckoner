#include "NumericValueNode.h"

double NumericValueNode::calculate()
{
    return value;
}

NodeType NumericValueNode::getNodeType()
{
    return NodeType::NumericValue;
}

std::string NumericValueNode::serializeToJson(int level)
{
    std::string tabs = std::string(level, '\t');
    std::stringstream ss;
    
    ss << "{" << std::endl;
    ss << tabs << "\t\"type\": \"NumericValue\"," << std::endl;
    ss << tabs << "\t\"value\": \"" << value << "\"" << std::endl;
    ss << tabs << "}";

    return ss.str();
}
