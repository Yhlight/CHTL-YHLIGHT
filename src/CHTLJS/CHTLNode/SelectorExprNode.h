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

    std::unique_ptr<ExprNode> clone_expr() const override {
        auto node = std::make_unique<SelectorExprNode>();
        node->type = this->type;
        node->baseName = this->baseName;
        node->descendant = this->descendant;
        node->index = this->index;
        return node;
    }

    SelectorType type;
    std::string baseName;
    std::string descendant;
    std::optional<int> index;
};

} // namespace CHTLJS
