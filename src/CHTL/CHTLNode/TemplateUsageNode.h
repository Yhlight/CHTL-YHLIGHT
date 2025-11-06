#pragma once

#include "ASTNode.h"
#include <string>

class TemplateUsageNode : public ASTNode {
public:
    explicit TemplateUsageNode(std::string name)
        : ASTNode(ASTNodeType::TemplateUsage), name_(std::move(name)) {}

    const std::string& getName() const { return name_; }

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<TemplateUsageNode>(name_);
    }

private:
    std::string name_;
};
