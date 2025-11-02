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
};

} // namespace CHTLJS
