#pragma once

#include "ValueNode.h"
#include <string>
#include <memory>

namespace CHTL {

class PropertyReferenceNode : public ValueNode {
public:
    std::string selector;
    std::string propertyName;

    PropertyReferenceNode(const std::string& selector, const std::string& propertyName)
        : selector(selector), propertyName(propertyName) {}

    NodeType getType() const override { return NodeType::PropertyReference; }

    std::unique_ptr<ValueNode> clone() const override {
        return std::make_unique<PropertyReferenceNode>(selector, propertyName);
    }

    virtual std::string ToString() const override {
        return "PropertyReferenceNode(" + selector + "." + propertyName + ")";
    }
};

}
