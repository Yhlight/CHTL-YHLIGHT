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

    std::string selector;
    std::vector<std::unique_ptr<StyleContentNode>> children;
};
