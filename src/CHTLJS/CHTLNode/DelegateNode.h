#pragma once

#include "ExprNode.h"
#include "EventHandlerNode.h"
#include "SelectorExprNode.h"
#include <vector>
#include <memory>

namespace CHTLJS {

class DelegateNode : public ExprNode {
public:
    std::unique_ptr<SelectorExprNode> parentSelector;
    std::unique_ptr<SelectorExprNode> targetSelector;
    std::vector<std::unique_ptr<EventHandlerNode>> handlers;

    DelegateNode(std::unique_ptr<SelectorExprNode> parentSelector,
                 std::unique_ptr<SelectorExprNode> targetSelector,
                 std::vector<std::unique_ptr<EventHandlerNode>>&& handlers)
        : parentSelector(std::move(parentSelector)),
          targetSelector(std::move(targetSelector)),
          handlers(std::move(handlers)) {}

    ASTNodeType getType() const override { return ASTNodeType::Delegate; }

    std::unique_ptr<ExprNode> clone_expr() const override {
        auto newParentSelector = std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(parentSelector->clone().release()));
        auto newTargetSelector = std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(targetSelector->clone().release()));
        std::vector<std::unique_ptr<EventHandlerNode>> newHandlers;
        for (const auto& handler : handlers) {
            newHandlers.push_back(std::unique_ptr<EventHandlerNode>(static_cast<EventHandlerNode*>(handler->clone().release())));
        }
        return std::make_unique<DelegateNode>(std::move(newParentSelector), std::move(newTargetSelector), std::move(newHandlers));
    }
};

} // namespace CHTLJS
