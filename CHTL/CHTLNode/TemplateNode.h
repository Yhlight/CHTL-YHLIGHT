#pragma once

#include "BaseNode.h"
#include <string>
#include <map>

class TemplateNode : public BaseNode {
public:
    std::string name;
    std::string type; // @Style, @Element, @Var
    AstNodeList body;
    std::map<std::string, std::string> variables;
};
