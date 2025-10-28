#pragma once

#include "BaseNode.h"
#include "AttributeNode.h"
#include "StyleBlockNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class ElementNode : public BaseNode {
public:
    ElementNode(const std::string& tagName) : m_tagName(tagName) {}

    NodeType getType() const override { return NodeType::Element; }

    const std::string& getTagName() const { return m_tagName; }
    const std::vector<std::shared_ptr<AttributeNode>>& getAttributes() const { return m_attributes; }
    const std::vector<std::shared_ptr<BaseNode>>& getChildren() const { return m_children; }
    std::shared_ptr<StyleBlockNode> getStyleBlock() const { return m_styleBlock; }

    void addAttribute(std::shared_ptr<AttributeNode> attribute) {
        m_attributes.push_back(attribute);
    }

    void addChild(std::shared_ptr<BaseNode> child) {
        m_children.push_back(child);
    }

    void setStyleBlock(std::shared_ptr<StyleBlockNode> styleBlock) {
        m_styleBlock = styleBlock;
    }

private:
    std::string m_tagName;
    std::vector<std::shared_ptr<AttributeNode>> m_attributes;
    std::vector<std::shared_ptr<BaseNode>> m_children;
    std::shared_ptr<StyleBlockNode> m_styleBlock;
};

} // namespace CHTL
