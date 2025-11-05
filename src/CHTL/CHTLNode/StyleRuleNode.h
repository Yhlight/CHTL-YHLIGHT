#pragma once

#include "StyleContentNode.h"
#include <string>
#include <vector>
#include <memory>

class StyleRuleNode : public StyleContentNode {
public:
    StyleRuleNode(const std::string& selector) : selector(selector) {}

    NodeType getType() const override { return NodeType::StyleContent; }
    StyleContentType getStyleContentType() const override { return StyleContentType::Rule; }

    std::unique_ptr<BaseNode> clone() const override {
        auto node = std::make_unique<StyleRuleNode>(selector);
        for (const auto& child : children) {
            node->children.push_back(std::unique_ptr<StyleContentNode>(static_cast<StyleContentNode*>(child->clone().release())));
        }
        return node;
    }

    std::string selector;
    std::vector<std::unique_ptr<StyleContentNode>> children;
};
