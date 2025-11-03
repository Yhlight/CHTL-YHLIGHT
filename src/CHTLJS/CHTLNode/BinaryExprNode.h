#pragma once

#include "ExprNode.h"
#include "../CHTLLexer/Token.h"

namespace CHTLJS {

class BinaryExprNode : public ExprNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::BinaryExpr; }

    std::unique_ptr<ExprNode> left;
    Token op;
    std::unique_ptr<ExprNode> right;

    std::unique_ptr<ExprNode> clone_expr() const override {
        auto node = std::make_unique<BinaryExprNode>();
        node->left = std::unique_ptr<ExprNode>(static_cast<ExprNode*>(left->clone().release()));
        node->op = op;
        node->right = std::unique_ptr<ExprNode>(static_cast<ExprNode*>(right->clone().release()));
        return node;
    }
};

} // namespace CHTLJS
