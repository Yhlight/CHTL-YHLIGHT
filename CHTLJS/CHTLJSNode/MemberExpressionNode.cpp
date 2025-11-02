#include "MemberExpressionNode.h"

MemberExpressionNode::MemberExpressionNode(std::unique_ptr<ExpressionNode> object, std::unique_ptr<ExpressionNode> property)
    : object(std::move(object)), property(std::move(property)) {}
