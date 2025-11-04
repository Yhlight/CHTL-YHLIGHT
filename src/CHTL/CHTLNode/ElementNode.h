#pragma once

#include "ASTNode.h"
#include <string>
#include <map>

namespace CHTL {

class ElementNode : public ASTNode {
public:
    ElementNode(const std::string& tagName) : tagName(tagName) {}

    ASTNodeType getType() const override { return ASTNodeType::Element; }

    std::string tagName;
    std::map<std::string, std::string> attributes;
};

} // namespace CHTL
