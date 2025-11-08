#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class NamespaceNode : public BaseNode {
public:
    std::string name;
    std::vector<std::unique_ptr<BaseNode>> body;

    NamespaceNode(const std::string& name) : name(name) {}

    NodeType getType() const override { return NodeType::Namespace; }
    std::string ToString() const override {
        return "NamespaceNode: [name=" + name + "]";
    }
};

} // namespace CHTL
