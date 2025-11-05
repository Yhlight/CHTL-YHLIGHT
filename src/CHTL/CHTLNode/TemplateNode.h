#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

enum class TemplateType {
    Element,
    Style,
    Var,
};

class TemplateNode : public BaseNode {
public:
    TemplateNode(const std::string& name, TemplateType type) : name(name), type(type) {}

    NodeType getType() const override { return NodeType::Template; }

    std::string name;
    TemplateType type;
    std::vector<std::unique_ptr<BaseNode>> children;
};
