#pragma once

#include "ValueNode.h"
#include <string>
#include <memory>

namespace CHTL {

class LogicalNode : public ValueNode {
public:
    std::unique_ptr<ValueNode> left;
    std::string op;
    std::unique_ptr<ValueNode> right;

    NodeType getType() const override { return NodeType::Logical; }

    std::unique_ptr<ValueNode> clone() const override {
        auto node = std::make_unique<LogicalNode>();
        node->left = left->clone();
        node->op = op;
        node->right = right->clone();
        return node;
    }

    virtual std::string ToString() const override {
        return "LogicalNode(" + left->ToString() + " " + op + " " + right->ToString() + ")";
    }
};

}
