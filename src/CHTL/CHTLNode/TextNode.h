#pragma once

#include "ASTNode.h"
#include <string>

class TextNode : public ASTNode {
public:
    explicit TextNode(std::string text) : ASTNode(ASTNodeType::Text), text_(std::move(text)) {}

    const std::string& getText() const { return text_; }

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<TextNode>(text_);
    }

private:
    std::string text_;
};
