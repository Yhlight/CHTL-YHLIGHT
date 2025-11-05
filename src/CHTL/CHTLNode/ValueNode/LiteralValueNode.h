#pragma once

#include "ValueNode.h"

namespace CHTL {

    class LiteralValueNode : public ValueNode {
    public:
        std::string value;

        LiteralValueNode(const std::string& val)
            : ValueNode(ValueNodeType::Literal, ASTNodeType::Value), value(val) {}

        virtual std::unique_ptr<ASTNode> clone() const override {
            return std::make_unique<LiteralValueNode>(value);
        }
    };

}
