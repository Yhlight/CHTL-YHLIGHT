#pragma once

#include "BaseNode.h"
#include <string>

class EnhancedSelectorNode : public CHTLJSBaseNode {
public:
    EnhancedSelectorNode(const std::string& selector);
    std::string selector;
};
