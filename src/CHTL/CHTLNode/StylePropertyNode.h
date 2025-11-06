#pragma once

#include "ASTNode.h"
#include "ValueNode.h"
#include <string>
#include <memory>

class StylePropertyNode : public ASTNode {
public:
    StylePropertyNode(std::string name, std::unique_ptr<ValueNode> value)
        : ASTNode(ASTNodeType::StyleProperty), name_(std::move(name)), value_(std::move(value)) {}

    const std::string& getName() const { return name_; }
    const ValueNode* getValue() const { return value_.get(); }

private:
    std::string name_;
    std::unique_ptr<ValueNode> value_;
};
