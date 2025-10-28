#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class TemplateElementUsageNode : public BaseNode {
public:
    TemplateElementUsageNode(const std::string& name) : m_name(name) {}

    NodeType getType() const override { return NodeType::TemplateElementUsage; }

    const std::string& getName() const { return m_name; }

private:
    std::string m_name;
};

} // namespace CHTL
