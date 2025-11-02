#pragma once

#include "ASTNode.h"
#include "StyleNode.h"
#include <string>
#include <memory>

namespace CHTL {

class TemplateUsageNode : public ASTNode {
public:
    std::string name;
    std::unique_ptr<StyleNode> specialization;

    ASTNodeType getType() const override { return ASTNodeType::TemplateUsage; }
};

} // namespace CHTL
