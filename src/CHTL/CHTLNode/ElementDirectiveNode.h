#pragma once

#include "BaseNode.h"
#include <string>

class ElementDirectiveNode : public BaseNode {
public:
    ElementDirectiveNode(const std::string& name) : name(name) {}

    NodeType getType() const override { return NodeType::ElementDirective; }

    std::unique_ptr<BaseNode> clone() const override {
        auto node = std::make_unique<ElementDirectiveNode>(name);
        node->from_namespace = from_namespace;
        return node;
    }

    std::string name;
    std::string from_namespace;
};
