#pragma once

#include "ValueNode.h"
#include <string>
#include <memory>

namespace CHTL {

class ConditionalNode : public ValueNode {
public:
    std::unique_ptr<ValueNode> condition;
    std::unique_ptr<ValueNode> true_branch;
    std::unique_ptr<ValueNode> false_branch;

    NodeType getType() const override { return NodeType::Conditional; }

    std::unique_ptr<ValueNode> clone() const override {
        auto node = std::make_unique<ConditionalNode>();
        node->condition = condition->clone();
        node->true_branch = true_branch->clone();
        node->false_branch = false_branch->clone();
        return node;
    }

    virtual std::string ToString() const override {
        return "ConditionalNode(" + condition->ToString() + " ? " + true_branch->ToString() + " : " + false_branch->ToString() + ")";
    }
};

}
