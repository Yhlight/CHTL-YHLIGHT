#pragma once

#include "BaseNode.h"
#include <string>

class TemplateNode : public BaseNode {
public:
    std::string name;
    std::string type; // @Style, @Element, @Var
    AstNodeList body;
};
