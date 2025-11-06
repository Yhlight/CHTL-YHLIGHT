#pragma once

#include "ValueNode.h"
#include <string>

class LiteralValueNode : public ValueNode {
public:
    explicit LiteralValueNode(std::string value) : ValueNode(ASTNodeType::LiteralValue), value_(std::move(value)) {}

    const std::string& getValue() const { return value_; }

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<LiteralValueNode>(value_);
    }

private:
    std::string value_;
};
