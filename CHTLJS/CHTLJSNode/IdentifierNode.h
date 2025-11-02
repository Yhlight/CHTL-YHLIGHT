#pragma once

#include "ExpressionNode.h"
#include <string>

class IdentifierNode : public ExpressionNode {
public:
    IdentifierNode(const std::string& name);
    std::string name;
};
