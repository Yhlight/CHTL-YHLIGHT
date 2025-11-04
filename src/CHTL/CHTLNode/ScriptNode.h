#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class ScriptNode : public ASTNode {
public:
    ScriptNode(const std::string& content) : content(content) {}

    ASTNodeType getType() const override { return ASTNodeType::Script; }

    std::string content;
};

} // namespace CHTL
