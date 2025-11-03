#pragma once

#include "BaseNode.h"
#include <string>

class OriginDirectiveNode : public BaseNode {
public:
    std::string type; // @Html, @Style, @JavaScript
    std::string name;
};
