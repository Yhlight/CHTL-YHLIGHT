#pragma once

#include "ASTNode.h"

namespace CHTLJS {

class ExprNode : public ASTNode {
public:
    virtual ~ExprNode() = default;
    virtual std::unique_ptr<ExprNode> clone_expr() const = 0;

    std::unique_ptr<ASTNode> clone() const override {
        return clone_expr();
    }
};

} // namespace CHTLJS
