#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>
#include <map>

class ElementNode : public BaseNode {
public:
    ElementNode(const std::string& tag_name) : tag_name(tag_name) {}

    NodeType getType() const override { return NodeType::Element; }

    std::unique_ptr<BaseNode> clone() const override {
        auto node = std::make_unique<ElementNode>(tag_name);
        node->attributes = attributes;
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }

    std::string tag_name;
    std::map<std::string, std::string> attributes;
    std::vector<std::unique_ptr<BaseNode>> children;
};
