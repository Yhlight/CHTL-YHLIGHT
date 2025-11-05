#pragma once

#include "ValueNode.h"

namespace CHTL {

    class VariableUsageNode : public ValueNode {
    public:
        std::string name;

        VariableUsageNode(const std::string& name)
            : ValueNode(ValueNodeType::VariableUsage, ASTNodeType::Value), name(name) {}

        virtual std::unique_ptr<ASTNode> clone() const override {
            return std::make_unique<VariableUsageNode>(name);
        }
    };

}
