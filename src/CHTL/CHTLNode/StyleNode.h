#pragma once

#include "BaseNode.h"
#include "StyleContentNode.h"
#include <vector>
#include <memory>

class StyleNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Style; }

    std::unique_ptr<BaseNode> clone() const override {
        auto node = std::make_unique<StyleNode>();
        for (const auto& child : children) {
            node->children.push_back(std::unique_ptr<StyleContentNode>(static_cast<StyleContentNode*>(child->clone().release())));
        }
        return node;
    }

    std::vector<std::unique_ptr<StyleContentNode>> children;
};
