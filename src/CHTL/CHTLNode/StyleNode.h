#pragma once

#include "ASTNode.h"
#include <vector>
#include <memory>

namespace CHTL {

class StylePropertyNode;

class StyleNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::StyleBlock; }

    std::vector<std::unique_ptr<StylePropertyNode>> properties;
};

}
