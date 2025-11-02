#pragma once

#include "ASTNode.h"

namespace CHTL {

class ProgramNode : public ASTNode {
public:
    ASTNodeType getType() const override { return ASTNodeType::Program; }

    std::vector<std::unique_ptr<ASTNode>> children;
};

}
