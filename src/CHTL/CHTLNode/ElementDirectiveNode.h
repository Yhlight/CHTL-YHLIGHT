#pragma once

#include "BaseNode.h"
#include <string>

class ElementDirectiveNode : public BaseNode {
public:
    ElementDirectiveNode(const std::string& name) : name(name) {}

    NodeType getType() const override { return NodeType::ElementDirective; }

    std::string name;
};
