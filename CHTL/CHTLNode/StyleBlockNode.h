#pragma once

#include "BaseNode.h"
#include <string>

namespace CHTL {

class StyleBlockNode : public BaseNode {
public:
    StyleBlockNode(const std::string& content) : m_content(content) {}

    NodeType getType() const override { return NodeType::StyleBlock; }

    const std::string& getContent() const { return m_content; }

private:
    std::string m_content;
};

} // namespace CHTL
