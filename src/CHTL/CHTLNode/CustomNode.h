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

    std::string name;
    CustomType type;
    std::vector<std::string> valueless_properties;
};
