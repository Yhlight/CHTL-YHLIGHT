#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class TextNode : public ASTNode {
public:
    TextNode(const std::string& content) : content(content) {}

    ASTNodeType getType() const override { return ASTNodeType::Text; }
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<TextNode>(content);
    }

    std::string content;
};

} // namespace CHTL
