#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTL {

class StyleNode : public ASTNode {
public:
    StyleNode(const std::string& content) : content(content) {}

    ASTNodeType getType() const override { return ASTNodeType::Style; }

    std::string content;
};

} // namespace CHTL
