#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class ScriptNode : public ASTNode {
public:
    ScriptNode(const std::string& content) : content(content) {}

    ASTNodeType getType() const override { return ASTNodeType::Script; }
    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<ScriptNode>(content);
    }

    std::string content;
};

} // namespace CHTL
