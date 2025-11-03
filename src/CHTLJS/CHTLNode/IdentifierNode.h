#pragma once

#include "ExprNode.h"
#include <string>

namespace CHTLJS {

class IdentifierNode : public ExprNode {
public:
    IdentifierNode(const std::string& name) : m_name(name) {}

    ASTNodeType getType() const override { return ASTNodeType::Identifier; }

    const std::string& getName() const { return m_name; }

    std::unique_ptr<ExprNode> clone_expr() const override {
        return std::make_unique<IdentifierNode>(m_name);
    }

private:
    std::string m_name;
};

} // namespace CHTLJS
