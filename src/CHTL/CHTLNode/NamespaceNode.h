#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class NamespaceNode : public ASTNode {
public:
    NamespaceNode(const std::string& name)
        : ASTNode(ASTNodeType::Namespace), name(name) {}

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<NamespaceNode>(name);
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }

    std::string name;
};

} // namespace CHTL
