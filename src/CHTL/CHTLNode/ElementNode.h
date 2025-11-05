#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

class ElementNode : public BaseNode {
public:
    ElementNode(const std::string& tag_name) : tag_name(tag_name) {}

    NodeType getType() const override { return NodeType::Element; }

    std::string tag_name;
    std::vector<std::pair<std::string, std::string>> attributes;
    std::vector<std::unique_ptr<BaseNode>> children;
};
