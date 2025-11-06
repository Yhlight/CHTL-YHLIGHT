#pragma once

#include "ExprNode.h"
#include "CHTLJSLexer/Token.h"

namespace CHTLJS {

class UnaryExprNode : public ExprNode {
public:
    UnaryExprNode(Token op, std::unique_ptr<ExprNode> right)
        : ExprNode(ExprNodeType::Unary), op_(op), right_(std::move(right)) {}

    const Token& getOperator() const { return op_; }
    const ExprNode* getRight() const { return right_.get(); }

private:
    Token op_;
    std::unique_ptr<ExprNode> right_;
};

}
