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

    std::unique_ptr<BaseNode> clone() const override {
        auto node = std::make_unique<TemplateNode>(name, type);
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        node->variables = variables;
        return node;
    }

    std::string name;
    TemplateType type;
    std::vector<std::unique_ptr<BaseNode>> children;
    std::map<std::string, std::string> variables;
};
