#pragma once

#include "BaseNode.h"
#include <string>
#include <map>

class ElementNode : public BaseNode {
public:
    std::string tag_name;
    std::map<std::string, std::string> attributes;
    AstNodeList children;
};
