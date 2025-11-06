#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class ElementNode : public BaseNode {
public:
    ElementNode(const std::string& tagName) : tagName(tagName) {}

    NodeType getType() const override { return NodeType::Element; }

    std::string tagName;
    std::vector<std::unique_ptr<BaseNode>> children;
};

} // namespace CHTL
