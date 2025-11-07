#pragma once

#include "StyleContentNode.h"
#include <string>

namespace CHTL {

class TemplateUsageNode : public StyleContentNode {
public:
    TemplateUsageNode(const std::string& type, const std::string& name) : type(type), name(name) {}

    NodeType getType() const override { return NodeType::TemplateUsage; }

    std::string type;
    std::string name;
};

} // namespace CHTL
