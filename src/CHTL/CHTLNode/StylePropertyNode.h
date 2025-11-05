#pragma once

#include "ASTNode.h"
#include "ValueNode/ValueNode.h"
#include <string>
#include <memory>

namespace CHTL {

class StylePropertyNode : public ASTNode {
public:
    StylePropertyNode(const std::string& name, std::unique_ptr<ValueNode> value)
        : ASTNode(ASTNodeType::StyleProperty), name(name), value(std::move(value)) {}

    std::unique_ptr<ASTNode> clone() const override {
        auto cloned_value = std::unique_ptr<ValueNode>(static_cast<ValueNode*>(value->clone().release()));
        return std::make_unique<StylePropertyNode>(name, std::move(cloned_value));
    }

    std::string name;
    std::unique_ptr<ValueNode> value;
};

} // namespace CHTL
