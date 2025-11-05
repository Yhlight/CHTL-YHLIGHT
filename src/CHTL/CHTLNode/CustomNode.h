#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>

enum class CustomType {
    Style,
    Element,
    Var,
};

class CustomNode : public BaseNode {
public:
    CustomNode(const std::string& name, CustomType type) : name(name), type(type) {}

    NodeType getType() const override { return NodeType::Custom; }

    std::unique_ptr<BaseNode> clone() const override {
        auto node = std::make_unique<CustomNode>(name, type);
        node->valueless_properties = valueless_properties;
        return node;
    }

    std::string name;
    CustomType type;
    std::vector<std::string> valueless_properties;
};
