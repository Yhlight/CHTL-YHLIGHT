#pragma once

#include "BaseNode.h"
#include <map>
#include <string>

class StyleNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::Style; }

    std::map<std::string, std::string> properties;
};
