#pragma once

#include "BaseNode.h"
#include <string>

class NamespaceNode : public BaseNode {
public:
    std::string name;
    AstNodeList body;
};
