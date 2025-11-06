#pragma once

#include "ASTNode.h"
#include <string>

enum class TemplateType {
    Style,
    Element,
    Var
};

class TemplateNode : public ASTNode {
public:
    TemplateNode(std::string name, TemplateType type)
        : ASTNode(ASTNodeType::Template), name_(std::move(name)), templateType_(type) {}

    const std::string& getName() const { return name_; }
    TemplateType getTemplateType() const { return templateType_; }

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<TemplateNode>(name_, templateType_);
        for (const auto& child : children_) {
            node->addChild(child->clone());
        }
        return node;
    }

private:
    std::string name_;
    TemplateType templateType_;
};
