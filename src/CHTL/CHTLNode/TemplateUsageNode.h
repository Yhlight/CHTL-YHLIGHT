#pragma once

#include "StyleContentNode.h"
#include "StylePropertyNode.h"
#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class TemplateUsageNode : public StyleContentNode {
public:
    TemplateUsageNode(const std::string& type, const std::string& name) : type(type), name(name) {}

    NodeType getType() const override { return NodeType::TemplateUsage; }

    std::string type;
    std::string name;
    std::string from;
    bool deleted = false;
    std::vector<std::unique_ptr<StylePropertyNode>> provided_properties;
    std::vector<std::unique_ptr<BaseNode>> body;
};

} // namespace CHTL
