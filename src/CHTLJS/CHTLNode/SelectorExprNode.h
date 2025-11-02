#pragma once

#include "ExprNode.h"
#include <string>

namespace CHTLJS {

class SelectorExprNode : public ExprNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::SelectorExpr; }

    std::string selector;
};

} // namespace CHTLJS
