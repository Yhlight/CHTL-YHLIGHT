#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class TextNode : public BaseNode {
public:
    TextNode(const std::string& content) : content(content) {}

    NodeType getType() const override { return NodeType::Text; }
    std::unique_ptr<BaseNode> clone() const override;

    std::string content;
};

} // namespace CHTL
