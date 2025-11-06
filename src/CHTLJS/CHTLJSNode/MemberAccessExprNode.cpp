#include "MemberAccessExprNode.h"

namespace CHTLJS {

MemberAccessExprNode::MemberAccessExprNode(std::unique_ptr<ExprNode> object, Token member)
    : ExprNode(ExprNodeType::MemberAccess), object_(std::move(object)), member_(member) {}

const ExprNode& MemberAccessExprNode::getObject() const {
    return *object_;
}

const Token& MemberAccessExprNode::getMember() const {
    return member_;
}

}
