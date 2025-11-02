#pragma once

#include "ASTNode.h"

namespace CHTLJS {

class ExprNode : public ASTNode {
public:
    virtual ~ExprNode() = default;
};

} // namespace CHTLJS
