#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class ScriptNode : public ASTNode {
public:
    ScriptNode(const std::string& content)
        : ASTNode(ASTNodeType::Script), content(content) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<ScriptNode>(content);
    }

    std::string content;
};

} // namespace CHTL
