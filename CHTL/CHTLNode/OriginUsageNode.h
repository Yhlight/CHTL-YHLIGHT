#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class OriginUsageNode : public BaseNode {
public:
    OriginUsageNode(const std::string& type, const std::string& name)
        : m_type(type), m_name(name) {}

    NodeType getType() const override { return NodeType::OriginUsage; }

    const std::string& getOriginType() const { return m_type; }
    const std::string& getName() const { return m_name; }

private:
    std::string m_type;
    std::string m_name;
};

} // namespace CHTL
