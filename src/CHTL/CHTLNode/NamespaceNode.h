#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

class NamespaceNode : public BaseNode {
public:
    NamespaceNode(const std::string& name) : name(name) {}

    NodeType getType() const override { return NodeType::Namespace; }

    std::unique_ptr<BaseNode> clone() const override {
        auto node = std::make_unique<NamespaceNode>(name);
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }

    std::string name;
    std::vector<std::unique_ptr<BaseNode>> children;
};
