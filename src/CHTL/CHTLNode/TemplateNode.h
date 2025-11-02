#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

enum class TemplateType {
    Style,
    Element,
    Var,
};

class TemplateNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Template; }

    TemplateType templateType;
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> children;
};

}
