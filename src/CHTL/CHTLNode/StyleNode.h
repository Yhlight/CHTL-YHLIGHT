#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class StyleNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Style; }
};

} // namespace CHTL
