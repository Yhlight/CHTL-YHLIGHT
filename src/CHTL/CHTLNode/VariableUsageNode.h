#pragma once

#include "ValueNode.h"
#include <string>

namespace CHTL {

class VariableUsageNode : public ValueNode {
public:
    VariableUsageNode(const std::string& groupName, const std::string& variableName)
        : groupName(groupName), variableName(variableName) {}

    NodeType getType() const override { return NodeType::VariableUsage; }

    std::unique_ptr<ValueNode> clone() const override {
        return std::make_unique<VariableUsageNode>(groupName, variableName);
    }

    std::string groupName;
    std::string variableName;
    std::string from;
};

} // namespace CHTL
