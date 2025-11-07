#pragma once

#pragma once

#include "StyleContentNode.h"
#include "ValueNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class StylePropertyNode : public StyleContentNode {
public:
    StylePropertyNode(const std::string& key, std::vector<std::unique_ptr<ValueNode>> value)
        : key(key), value(std::move(value)) {}

    NodeType getType() const override { return NodeType::StyleProperty; }

    std::string key;
    std::vector<std::unique_ptr<ValueNode>> value;
};

} // namespace CHTL
