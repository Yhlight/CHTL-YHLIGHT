#pragma once

#include "ASTNode.h"
#include "ExprNode.h"
#include <vector>
#include <memory>

namespace CHTLJS {

class ProgramNode : public ASTNode {
public:
    ProgramNode(std::vector<std::unique_ptr<ExprNode>> statements)
        : statements(std::move(statements)) {}

    ASTNodeType getType() const override { return ASTNodeType::Program; }

    std::vector<std::unique_ptr<ExprNode>> statements;

    std::unique_ptr<ASTNode> clone() const override {
        std::vector<std::unique_ptr<ExprNode>> newStatements;
        for (const auto& stmt : statements) {
            newStatements.push_back(std::unique_ptr<ExprNode>(static_cast<ExprNode*>(stmt->clone().release())));
        }
        return std::make_unique<ProgramNode>(std::move(newStatements));
    }
};

} // namespace CHTLJS
