#pragma once

#include "ExpressionNode.h"
#include <string>

class StringLiteralNode : public ExpressionNode {
public:
    StringLiteralNode(const std::string& value);
    std::string value;
};
