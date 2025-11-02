#pragma once

#include "ASTNode.h"
#include <string>
#include <optional>

namespace CHTL {

class StylePropertyNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::StyleProperty; }

    std::string name;
    std::optional<std::string> value;
};

}
