#pragma once

#include "ExprNode.h"
#include <string>
#include <memory>

namespace CHTLJS {

class LiteralNode : public ExprNode {
public:
    LiteralNode() = default;
    LiteralNode(std::string value) : value(std::move(value)) {}

    ASTNodeType getType() const override { return ASTNodeType::Literal; }
    std::unique_ptr<ExprNode> clone_expr() const override {
		return std::make_unique<LiteralNode>(value);
	}
    std::unique_ptr<ASTNode> clone() const override {
		return std::make_unique<LiteralNode>(value);
	}

    std::string value;
};

} // namespace CHTLJS
