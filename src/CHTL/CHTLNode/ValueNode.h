#pragma once

#include "ASTNode.h"

class ValueNode : public ASTNode {
public:
    explicit ValueNode(ASTNodeType type) : ASTNode(type) {}
    virtual std::unique_ptr<ASTNode> clone() const = 0;
};
