#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class TemplateUsageNode : public ASTNode {
public:
    TemplateUsageNode(const std::string& type, const std::string& name) : templateType(type), name(name) {}

    ASTNodeType getType() const override { return ASTNodeType::TemplateUsage; }
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<TemplateUsageNode>(templateType, name);
    }

    std::string templateType;
    std::string name;
};

} // namespace CHTL
