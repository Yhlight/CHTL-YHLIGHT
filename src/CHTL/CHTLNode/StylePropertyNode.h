#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class StylePropertyNode : public ASTNode {
public:
    StylePropertyNode(const std::string& name, const std::string& value) : name(name), value(value) {}

    ASTNodeType getType() const override { return ASTNodeType::StyleProperty; }

    std::string name;
    std::string value;
};

} // namespace CHTL
