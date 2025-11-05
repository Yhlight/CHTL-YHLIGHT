#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class TemplateNode : public ASTNode {
public:
    TemplateNode(const std::string& type, const std::string& name) : templateType(type), name(name) {}

    ASTNodeType getType() const override { return ASTNodeType::Template; }

    std::string templateType;
    std::string name;
};

} // namespace CHTL
