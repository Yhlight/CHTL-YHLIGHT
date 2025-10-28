#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class ElementNode : public BaseNode {
public:
    ElementNode(const std::string& tagName) : m_tagName(tagName) {}

    NodeType getType() const override { return NodeType::Element; }

    const std::string& getTagName() const { return m_tagName; }
    const std::vector<std::shared_ptr<BaseNode>>& getChildren() const { return m_children; }

    void addChild(std::shared_ptr<BaseNode> child) {
        m_children.push_back(child);
    }

private:
    std::string m_tagName;
    std::vector<std::shared_ptr<BaseNode>> m_children;
};

} // namespace CHTL
