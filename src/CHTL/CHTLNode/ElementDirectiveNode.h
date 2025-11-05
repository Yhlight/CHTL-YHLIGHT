#pragma once

#include "BaseNode.h"
#include <string>

class ElementDirectiveNode : public BaseNode {
public:
    ElementDirectiveNode(const std::string& name) : name(name) {}

    NodeType getType() const override { return NodeType::ElementDirective; }

    std::unique_ptr<BaseNode> clone() const override {
        return std::make_unique<ElementDirectiveNode>(name);
    }

    std::string name;
};
