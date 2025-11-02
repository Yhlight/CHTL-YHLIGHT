#pragma once

#include <string>
#include <unordered_map>
#include "CHTL/CHTLNode/TemplateNode.h"

namespace CHTL {

class SymbolTable {
public:
    void insertStyleTemplate(const std::string& name, TemplateNode* node) {
        m_styleTemplates[name] = node;
    }

    TemplateNode* lookupStyleTemplate(const std::string& name) {
        if (m_styleTemplates.count(name)) {
            return m_styleTemplates[name];
        }
        return nullptr;
    }

    const std::unordered_map<std::string, TemplateNode*>& getStyleTemplates() const {
        return m_styleTemplates;
    }

private:
    std::unordered_map<std::string, TemplateNode*> m_styleTemplates;
};

} // namespace CHTL
