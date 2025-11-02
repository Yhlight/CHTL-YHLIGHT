#pragma once

#include "ExpressionNode.h"
#include <string>

class EnhancedSelectorNode : public ExpressionNode {
public:
    EnhancedSelectorNode(const std::string& selector);
    std::string selector;
};
