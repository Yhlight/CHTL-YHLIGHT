#pragma once

#include "StyleContentNode.h"
#include <string>
#include <map>

class StyleDirectiveNode : public StyleContentNode {
public:
    StyleDirectiveNode(const std::string& name) : name(name) {}

    NodeType getType() const override { return NodeType::StyleDirective; }
    StyleContentType getStyleContentType() const override { return StyleContentType::Directive; }

    std::unique_ptr<BaseNode> clone() const override {
        auto node = std::make_unique<StyleDirectiveNode>(name);
        node->properties = properties;
        node->from_namespace = from_namespace;
        return node;
    }

    std::string name;
    std::map<std::string, std::string> properties;
    std::string from_namespace;
};
