#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class StylePropertyNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::StyleProperty; }

    std::string name;
    std::string value;
};

}
