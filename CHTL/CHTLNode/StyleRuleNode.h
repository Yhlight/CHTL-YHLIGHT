#pragma once

#include "BaseNode.h"
#include "StylePropertyNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class StyleRuleNode : public BaseNode {
public:
    StyleRuleNode(const std::string& selector) : m_selector(selector) {}

    NodeType getType() const override { return NodeType::StyleRule; }
    const std::string& getSelector() const { return m_selector; }
    const std::vector<std::shared_ptr<StylePropertyNode>>& getProperties() const { return m_properties; }

    void addProperty(const std::shared_ptr<StylePropertyNode>& property) {
        m_properties.push_back(property);
    }

private:
    std::string m_selector;
    std::vector<std::shared_ptr<StylePropertyNode>> m_properties;
};

} // namespace CHTL
