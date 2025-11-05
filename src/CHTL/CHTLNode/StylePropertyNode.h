#pragma once

#include "StyleContentNode.h"
#include <string>

class StylePropertyNode : public StyleContentNode {
public:
    StylePropertyNode(const std::string& key, const std::string& value) : key(key), value(value) {}

    NodeType getType() const override { return NodeType::StyleContent; }
    StyleContentType getStyleContentType() const override { return StyleContentType::Property; }

    std::string key;
    std::string value;
};
