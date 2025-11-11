#pragma once

#include "ValueNode.h"
#include <string>

namespace CHTL {

class PropertyReferenceNode : public ValueNode {
public:
    PropertyReferenceNode(const std::string& selector, const std::string& propertyName)
        : selector(selector), propertyName(propertyName) {}

    NodeType getType() const override { return NodeType::PropertyReference; }
    std::unique_ptr<ValueNode> clone() const override {
        return std::make_unique<PropertyReferenceNode>(selector, propertyName);
    }

    std::string selector;
    std::string propertyName;
};

} // namespace CHTL
