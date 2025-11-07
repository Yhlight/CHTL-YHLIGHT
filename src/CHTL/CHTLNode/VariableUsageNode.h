#pragma once

#include "ValueNode.h"
#include <string>

namespace CHTL {

class VariableUsageNode : public ValueNode {
public:
    VariableUsageNode(const std::string& groupName, const std::string& variableName)
        : groupName(groupName), variableName(variableName) {}

    NodeType getType() const override { return NodeType::VariableUsage; }

    std::string groupName;
    std::string variableName;
};

} // namespace CHTL
