#pragma once

#include "ExpressionNode.h"
#include <memory>

class MemberExpressionNode : public ExpressionNode {
public:
    MemberExpressionNode(std::unique_ptr<ExpressionNode> object, std::unique_ptr<ExpressionNode> property);
    std::unique_ptr<ExpressionNode> object;
    std::unique_ptr<ExpressionNode> property;
};
