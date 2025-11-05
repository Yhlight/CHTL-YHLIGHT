#pragma once

#include "ASTNode.h"
#include <string>
#include <vector>
#include <optional>

namespace CHTL {

class OriginNode : public ASTNode {
public:
    std::string type;
    std::optional<std::string> name;
    std::string content;

    OriginNode(const std::string& type, const std::optional<std::string>& name, const std::string& content)
        : ASTNode(ASTNodeType::Origin), type(type), name(name), content(content) {}

    std::unique_ptr<ASTNode> clone() const override {
        auto node = std::make_unique<OriginNode>(type, name, content);
        for (const auto& child : children) {
            node->children.push_back(child->clone());
        }
        return node;
    }
};

}
