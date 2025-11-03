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
};

} // namespace CHTLJS
