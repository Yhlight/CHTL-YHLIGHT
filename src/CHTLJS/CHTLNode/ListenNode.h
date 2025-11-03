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
};

} // namespace CHTLJS
