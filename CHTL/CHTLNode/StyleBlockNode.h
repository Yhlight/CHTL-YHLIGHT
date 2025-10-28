#pragma once

#include "BaseNode.h"
#include <string>
#include <vector>

namespace CHTL {

class StyleBlockNode : public BaseNode {
public:
    StyleBlockNode(const std::string& content) : m_rawContent(content) {}

    NodeType getType() const override { return NodeType::StyleBlock; }

    const std::string& getRawContent() const { return m_rawContent; }
    const std::vector<std::string>& getUsedTemplates() const { return m_usedTemplates; }

    void addUsedTemplate(const std::string& templateName) {
        m_usedTemplates.push_back(templateName);
    }

private:
    std::string m_rawContent;
    std::vector<std::string> m_usedTemplates;
};

} // namespace CHTL
