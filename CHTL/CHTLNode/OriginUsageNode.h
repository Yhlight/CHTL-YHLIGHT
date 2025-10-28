
#pragma once

#include "BaseNode.h"
#include "OriginNode.h" // For OriginType
#include <string>

namespace CHTL {

class OriginUsageNode : public BaseNode {
public:
    OriginUsageNode(const std::string& name) : m_name(name) {}

    NodeType getType() const override { return NodeType::OriginUsage; }
    const std::string& getName() const { return m_name; }

private:
    std::string m_name;
};

} // namespace CHTL
