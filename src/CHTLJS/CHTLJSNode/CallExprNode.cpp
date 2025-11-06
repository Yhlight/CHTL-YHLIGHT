#include "CallExprNode.h"

namespace CHTLJS {

CallExprNode::CallExprNode(std::unique_ptr<ExprNode> callee, Token paren, std::vector<std::unique_ptr<ExprNode>> arguments)
    : ExprNode(ExprNodeType::Call), callee_(std::move(callee)), paren_(paren), arguments_(std::move(arguments)) {}

const ExprNode& CallExprNode::getCallee() const {
    return *callee_;
}

const std::vector<std::unique_ptr<ExprNode>>& CallExprNode::getArguments() const {
    return arguments_;
}

}
