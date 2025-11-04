#pragma once

#include "ExprNode.h"
#include <memory>

namespace CHTLJS {

struct MemberExprNode : public ExprNode {
    MemberExprNode() = default;
    MemberExprNode(std::unique_ptr<ExprNode> object, std::unique_ptr<ExprNode> property)
        : object(std::move(object)), property(std::move(property)) {}

    ASTNodeType getType() const override { return ASTNodeType::MemberExpr; }
    std::unique_ptr<ExprNode> clone_expr() const override {
        return std::make_unique<MemberExprNode>(object->clone_expr(), property->clone_expr());
    }
    std::unique_ptr<ASTNode> clone() const override {
		return std::make_unique<MemberExprNode>(object->clone_expr(), property->clone_expr());
	}

    std::unique_ptr<ExprNode> object;
    std::unique_ptr<ExprNode> property;
};

} // namespace CHTLJS
