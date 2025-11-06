#pragma once

#include "StyleContentNode.h"
#include <string>

namespace CHTL {

class StylePropertyNode : public StyleContentNode {
public:
    StylePropertyNode(const std::string& key, const std::string& value) : key(key), value(value) {}

    NodeType getType() const override { return NodeType::StyleProperty; }

    std::string key;
    std::string value;
};

} // namespace CHTL
