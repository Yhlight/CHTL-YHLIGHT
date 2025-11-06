#pragma once

#include "ASTNode.h"
#include <string>

class TextNode : public ASTNode {
public:
    explicit TextNode(std::string text) : ASTNode(ASTNodeType::Text), text_(std::move(text)) {}

    const std::string& getText() const { return text_; }

private:
    std::string text_;
};
