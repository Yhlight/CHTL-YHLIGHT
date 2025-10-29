#pragma once

#include "BaseNode.h"
#include "StylePropertyNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class CssRuleNode : public BaseNode {
public:
    CssRuleNode(const std::string& selector) : m_selector(selector) {}

    NodeType getType() const override { return NodeType::CssRule; }
    const std::string& getSelector() const { return m_selector; }
    const std::vector<std::shared_ptr<StylePropertyNode>>& getProperties() const { return m_properties; }

    void addProperty(std::shared_ptr<StylePropertyNode> prop) {
        m_properties.push_back(prop);
    }

private:
    std::string m_selector;
    std::vector<std::shared_ptr<StylePropertyNode>> m_properties;
};

} // namespace CHTL
