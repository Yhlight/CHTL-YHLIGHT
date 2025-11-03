#pragma once

#include "ExprNode.h"
#include <string>
#include <memory>

namespace CHTLJS {

class LiteralNode : public ExprNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Literal; }

    std::string value;
};

} // namespace CHTLJS
