#pragma once

#include "ValueNode.h"
#include <string>
#include <memory>

namespace CHTL {

class BinaryOperationNode : public ValueNode {
public:
    BinaryOperationNode(std::unique_ptr<ValueNode> left, const std::string& op, std::unique_ptr<ValueNode> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    NodeType getType() const override { return NodeType::BinaryOperation; }
    std::unique_ptr<ValueNode> clone() const override {
        return std::make_unique<BinaryOperationNode>(left->clone(), op, right->clone());
    }

    std::unique_ptr<ValueNode> left;
    std::string op;
    std::unique_ptr<ValueNode> right;
};

} // namespace CHTL
