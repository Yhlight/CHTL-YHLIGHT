#pragma once

#include "ExprNode.h"
#include <string>
#include <memory>

namespace CHTLJS {

class LiteralNode : public ExprNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Literal; }

    std::unique_ptr<ExprNode> clone_expr() const override {
        auto node = std::make_unique<LiteralNode>();
        node->value = this->value;
        return node;
    }

    std::string value;
};

} // namespace CHTLJS
