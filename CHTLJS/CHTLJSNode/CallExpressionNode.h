#pragma once

#include "ExpressionNode.h"
#include <vector>
#include <memory>

class CallExpressionNode : public ExpressionNode {
public:
    CallExpressionNode(std::unique_ptr<ExpressionNode> callee, std::vector<std::unique_ptr<ExpressionNode>> arguments);
    std::unique_ptr<ExpressionNode> callee;
    std::vector<std::unique_ptr<ExpressionNode>> arguments;
};
