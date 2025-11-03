#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTLJS {

class CssPropertyNode : public ASTNode {
public:
    std::string name;
    std::string value;

    CssPropertyNode(const std::string& name, const std::string& value)
        : name(name), value(value) {}

    ASTNodeType getType() const override { return ASTNodeType::CssProperty; }

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<CssPropertyNode>(name, value);
    }
};

} // namespace CHTLJS
