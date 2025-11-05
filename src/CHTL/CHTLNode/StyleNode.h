#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class StyleNode : public ASTNode {
public:
    StyleNode() : ASTNode(ASTNodeType::Style) {}

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<StyleNode>();
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }
};

} // namespace CHTL
