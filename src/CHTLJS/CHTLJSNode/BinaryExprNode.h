#pragma once

#include "ExprNode.h"
#include "CHTLJSLexer/Token.h"

namespace CHTLJS {

class BinaryExprNode : public ExprNode {
public:
    BinaryExprNode(std::unique_ptr<ExprNode> left, Token op, std::unique_ptr<ExprNode> right)
        : ExprNode(ExprNodeType::Binary), left_(std::move(left)), op_(op), right_(std::move(right)) {}

    const ExprNode* getLeft() const { return left_.get(); }
    const Token& getOperator() const { return op_; }
    const ExprNode* getRight() const { return right_.get(); }

private:
    std::unique_ptr<ExprNode> left_;
    Token op_;
    std::unique_ptr<ExprNode> right_;
};

}
