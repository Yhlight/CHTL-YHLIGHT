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
    SelectorExprNode() = default;
    SelectorExprNode(SelectorType type, std::string baseName, std::string descendant = "", std::optional<int> index = std::nullopt)
        : type(type), baseName(std::move(baseName)), descendant(std::move(descendant)), index(index) {}


    ASTNodeType getType() const override { return ASTNodeType::SelectorExpr; }
    std::unique_ptr<ExprNode> clone_expr() const override {
        auto clone = std::make_unique<SelectorExprNode>(type, baseName, descendant, index);
        return clone;
    }
    std::unique_ptr<ASTNode> clone() const override {
		auto clone = std::make_unique<SelectorExprNode>(type, baseName, descendant, index);
		return clone;
	}


    SelectorType type;
    std::string baseName;
    std::string descendant;
    std::optional<int> index;
};

} // namespace CHTLJS
