#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>
#include <map>

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
    std::map<std::string, std::string> variables;
};
