#pragma once

#include "ExprNode.h"
#include <string>
#include <optional>
#include <memory>

namespace CHTLJS {

enum class SelectorType {
    Tag,
    Id,
    Class,
    Compound,
};

class SelectorExprNode : public ExprNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::SelectorExpr; }

    SelectorType type;
    std::string baseName;
    std::string descendant;
    std::optional<int> index;
};

} // namespace CHTLJS
