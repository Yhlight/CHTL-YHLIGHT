#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class TextNode : public BaseNode {
public:
    TextNode(const std::string& text) : m_text(text) {}

    NodeType getType() const override { return NodeType::Text; }

    const std::string& getText() const { return m_text; }

private:
    std::string m_text;
};

} // namespace CHTL
