#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class TemplateNode : public ASTNode {
public:
    TemplateNode(const std::string& type, const std::string& name)
        : ASTNode(ASTNodeType::Template), templateType(type), name(name) {}

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<TemplateNode>(templateType, name);
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }

    std::string templateType;
    std::string name;
};

} // namespace CHTL
