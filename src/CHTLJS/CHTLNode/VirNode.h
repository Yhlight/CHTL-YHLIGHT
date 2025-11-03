#pragma once

#include "ExprNode.h"
#include <string>
#include <memory>

namespace CHTLJS {

class VirNode : public ExprNode {
public:
    VirNode(const std::string& name, std::unique_ptr<ExprNode> expression)
        : m_name(name), m_expression(std::move(expression)) {}

    ASTNodeType getType() const override { return ASTNodeType::Vir; }

    const std::string& getName() const { return m_name; }
    const ExprNode* getExpression() const { return m_expression.get(); }

    std::unique_ptr<ExprNode> clone_expr() const override {
        return std::make_unique<VirNode>(m_name, std::unique_ptr<ExprNode>(static_cast<ExprNode*>(m_expression->clone().release())));
    }

private:
    std::string m_name;
    std::unique_ptr<ExprNode> m_expression;
};

} // namespace CHTLJS
