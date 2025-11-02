#pragma once

#include "BaseNode.h"
#include <string>

class OriginNode : public BaseNode {
public:
    std::string type; // @Html, @Style, @JavaScript
    std::string name; // Optional
    std::string content;
};
