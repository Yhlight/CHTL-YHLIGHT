#pragma once

#include "ASTNode.h"
#include <string>
#include <map>

namespace CHTL {

class ElementNode : public ASTNode {
public:
    ElementNode(const std::string& tagName)
        : ASTNode(ASTNodeType::Element), tagName(tagName) {}

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<ElementNode>(tagName);
        node->attributes = attributes;
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }

    std::string tagName;
    std::map<std::string, std::string> attributes;
};

} // namespace CHTL
