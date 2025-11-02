#pragma once

#include "BaseNode.h"
#include <string>

class CustomNode : public BaseNode {
public:
    std::string name;
    std::string type; // @Style, @Element, @Var
    AstNodeList body;
};
