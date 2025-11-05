#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

class NamespaceNode : public BaseNode {
public:
    NamespaceNode(std::string name, std::vector<std::unique_ptr<BaseNode>> children)
        : name(std::move(name)), children(std::move(children)) {}

    NodeType getType() const override { return NodeType::Namespace; }

    const std::string& getName() const { return name; }
    const std::vector<std::unique_ptr<BaseNode>>& getChildren() const { return children; }

    std::unique_ptr<BaseNode> clone() const override {
        std::vector<std::unique_ptr<BaseNode>> cloned_children;
        for (const auto& child : children) {
            cloned_children.push_back(child->clone());
        }
        return std::make_unique<NamespaceNode>(name, std::move(cloned_children));
    }

private:
    std::string name;
    std::vector<std::unique_ptr<BaseNode>> children;
};
