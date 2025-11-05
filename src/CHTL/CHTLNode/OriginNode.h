#pragma once

#include "ASTNode.h"
#include <string>
#include <optional>

namespace CHTL {

class OriginNode : public ASTNode {
public:
    OriginNode(const std::string& type, std::optional<std::string> name, const std::string& content)
        : ASTNode(ASTNodeType::Origin), type(type), name(std::move(name)), content(content) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<OriginNode>(type, name, content);
    }

    std::string type;
    std::optional<std::string> name;
    std::string content;
};

} // namespace CHTL
