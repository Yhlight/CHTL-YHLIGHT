#pragma once

#include "ExprNode.h"
#include "IdentifierNode.h"
#include <memory>

namespace CHTLJS {

class VirUsageNode : public ExprNode {
public:
    VirUsageNode(std::unique_ptr<ExprNode> selector, std::unique_ptr<IdentifierNode> vir)
        : selector(std::move(selector)), vir(std::move(vir)) {}

    ASTNodeType getType() const override { return ASTNodeType::VirUsage; }

    std::unique_ptr<ExprNode> selector;
    std::unique_ptr<IdentifierNode> vir;

    std::unique_ptr<ExprNode> clone_expr() const override {
        return std::make_unique<VirUsageNode>(
            std::unique_ptr<ExprNode>(static_cast<ExprNode*>(selector->clone().release())),
            std::unique_ptr<IdentifierNode>(static_cast<IdentifierNode*>(vir->clone().release()))
        );
    }
};

} // namespace CHTLJS
