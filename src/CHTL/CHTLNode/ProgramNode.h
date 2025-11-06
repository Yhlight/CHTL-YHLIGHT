#pragma once

#include "ASTNode.h"

class ProgramNode : public ASTNode {
public:
    ProgramNode() : ASTNode(ASTNodeType::Program) {}

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<ProgramNode>();
        for (const auto& child : children_) {
            node->addChild(child->clone());
        }
        return node;
    }
};
