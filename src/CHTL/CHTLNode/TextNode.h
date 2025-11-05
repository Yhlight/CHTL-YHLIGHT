#pragma once

#include "BaseNode.h"
#include <string>

class TextNode : public BaseNode {
public:
    TextNode(const std::string& text) : text(text) {}

    NodeType getType() const override { return NodeType::Text; }

    std::string text;
};
