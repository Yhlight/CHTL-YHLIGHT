#include "CallExpressionNode.h"

CallExpressionNode::CallExpressionNode(std::unique_ptr<ExpressionNode> callee, std::vector<std::unique_ptr<ExpressionNode>> arguments)
    : callee(std::move(callee)), arguments(std::move(arguments)) {}
