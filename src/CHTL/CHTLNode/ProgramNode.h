#pragma once

#include "ASTNode.h"

class ProgramNode : public ASTNode {
public:
    ProgramNode() : ASTNode(ASTNodeType::Program) {}
};
