#pragma once

#include "BaseNode.h"
#include <vector>
#include <memory>

class ProgramNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Program; }

    std::unique_ptr<BaseNode> clone() const override {
        auto node = std::make_unique<ProgramNode>();
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }

    std::vector<std::unique_ptr<BaseNode>> children;
};
