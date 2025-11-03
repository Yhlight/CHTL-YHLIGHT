#pragma once

#include "ExprNode.h"
#include "EventHandlerNode.h"
#include "SelectorExprNode.h"
#include <vector>
#include <memory>

namespace CHTLJS {

class ListenNode : public ExprNode {
public:
    std::unique_ptr<SelectorExprNode> selector;
    std::vector<std::unique_ptr<EventHandlerNode>> handlers;

    ListenNode(std::unique_ptr<SelectorExprNode> selector,
               std::vector<std::unique_ptr<EventHandlerNode>>&& handlers)
        : selector(std::move(selector)), handlers(std::move(handlers)) {}

    ASTNodeType getType() const override { return ASTNodeType::Listen; }

    std::unique_ptr<ExprNode> clone_expr() const override {
        auto newSelector = std::unique_ptr<SelectorExprNode>(static_cast<SelectorExprNode*>(selector->clone().release()));
        std::vector<std::unique_ptr<EventHandlerNode>> newHandlers;
        for (const auto& handler : handlers) {
            newHandlers.push_back(std::unique_ptr<EventHandlerNode>(static_cast<EventHandlerNode*>(handler->clone().release())));
        }
        return std::make_unique<ListenNode>(std::move(newSelector), std::move(newHandlers));
    }
};

} // namespace CHTLJS
