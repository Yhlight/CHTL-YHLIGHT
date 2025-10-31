#pragma once

#include "BaseNode.h"
#include "StylePropertyNode.h"
#include "StyleRuleNode.h"
#include <string>
#include <vector>
#include <memory>

namespace CHTL {

class StyleBlockNode : public BaseNode {
public:
    StyleBlockNode() = default;

    NodeType getType() const override { return NodeType::StyleBlock; }

    const std::vector<std::shared_ptr<StylePropertyNode>>& getProperties() const { return m_properties; }
    const std::vector<std::shared_ptr<StyleRuleNode>>& getRules() const { return m_rules; }
    const std::vector<std::string>& getUsedTemplates() const { return m_usedTemplates; }

    void addProperty(std::shared_ptr<StylePropertyNode> prop) {
        m_properties.push_back(prop);
    }

    void addRule(std::shared_ptr<StyleRuleNode> rule) {
        m_rules.push_back(rule);
    }

    void addUsedTemplate(const std::string& templateName) {
        m_usedTemplates.push_back(templateName);
    }

private:
    std::vector<std::shared_ptr<StylePropertyNode>> m_properties;
    std::vector<std::shared_ptr<StyleRuleNode>> m_rules;
    std::vector<std::string> m_usedTemplates;
};

} // namespace CHTL
