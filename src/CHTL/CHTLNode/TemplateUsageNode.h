#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class TemplateUsageNode : public ASTNode {
public:
    std::string name;

    ASTNodeType getType() const override { return ASTNodeType::TemplateUsage; }
};

} // namespace CHTL
