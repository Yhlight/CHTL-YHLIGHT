#pragma once

#include "BaseNode.h"
#include "StylePropertyNode.h"
#include "CssRuleNode.h"
#include <vector>
#include <string>
#include <memory>

namespace CHTL {

class StyleBlockNode : public BaseNode {
public:
    NodeType getType() const override { return NodeType::StyleBlock; }

    void addProperty(std::shared_ptr<StylePropertyNode> prop) { m_properties.push_back(prop); }
    const std::vector<std::shared_ptr<StylePropertyNode>>& getProperties() const { return m_properties; }

    void addUsedTemplate(const std::string& templateName) { m_usedTemplates.push_back(templateName); }
    const std::vector<std::string>& getUsedTemplates() const { return m_usedTemplates; }

    void addRule(std::shared_ptr<CssRuleNode> rule) { m_rules.push_back(rule); }
    const std::vector<std::shared_ptr<CssRuleNode>>& getRules() const { return m_rules; }

private:
    std::vector<std::shared_ptr<StylePropertyNode>> m_properties;
    std::vector<std::string> m_usedTemplates;
    std::vector<std::shared_ptr<CssRuleNode>> m_rules;
};

} // namespace CHTL
