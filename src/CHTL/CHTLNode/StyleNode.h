#pragma once

#include "ASTNode.h"

class StyleNode : public ASTNode {
public:
    StyleNode() : ASTNode(ASTNodeType::Style) {}
};
