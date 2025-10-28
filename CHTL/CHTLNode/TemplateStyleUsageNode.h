#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class TemplateStyleUsageNode : public BaseNode {
public:
    TemplateStyleUsageNode(const std::string& name) : m_name(name) {}

    NodeType getType() const override { return NodeType::TemplateStyleUsage; }

    const std::string& getName() const { return m_name; }

private:
    std::string m_name;
};

} // namespace CHTL
