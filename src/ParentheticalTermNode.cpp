#include "ParentheticalTermNode.h"

double ParentheticalTermNode::calculate()
{
    return subtreeRoot->calculate();
}

NodeType ParentheticalTermNode::getNodeType()
{
    return NodeType::ParentheticalTerm;
}

std::string ParentheticalTermNode::serializeToJson(int level)
{
    std::string tabs = std::string(level, '\t');
    std::stringstream ss;

    ss << "{" << std::endl;
    ss << tabs << "\t\"type\": \"ParentheticalTerm\"," << std::endl;
    ss << tabs << "\t\"subtreeRoot\": " << subtreeRoot->serializeToJson(level + 1) << std::endl;
    ss << tabs << "}";

    return ss.str();
}
