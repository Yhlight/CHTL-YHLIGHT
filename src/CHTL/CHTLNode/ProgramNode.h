#pragma once

#include "ASTNode.h"

namespace CHTL {

class ProgramNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Program; }
    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<ProgramNode>();
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }
};

} // namespace CHTL
