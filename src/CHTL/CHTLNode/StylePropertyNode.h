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
    StylePropertyNode() = default;
    StylePropertyNode(const std::string& key, std::vector<std::unique_ptr<ValueNode>> value)
        : key(key), value(std::move(value)) {}
    StylePropertyNode(const std::string& key)
        : key(key) {}

    NodeType getType() const override { return NodeType::StyleProperty; }
    std::unique_ptr<BaseNode> clone() const override;

    std::vector<std::unique_ptr<ValueNode>> getClonedValues() const {
        std::vector<std::unique_ptr<ValueNode>> cloned_values;
        for (const auto& v : value) {
            cloned_values.push_back(std::unique_ptr<ValueNode>(static_cast<ValueNode*>(v->clone().release())));
        }
        return cloned_values;
    }

    std::string key;
    std::vector<std::unique_ptr<ValueNode>> value;
};

} // namespace CHTL
