#include "SymbolTable.h"

namespace CHTL {

void SymbolTable::insert(const std::string& selector, const ElementNode* node) {
    m_symbols[selector] = {node, {}};
}

const Symbol* SymbolTable::find(const std::string& selector) const {
    auto it = m_symbols.find(selector);
    if (it != m_symbols.end()) {
        return &it->second;
    }
    return nullptr;
}

void SymbolTable::addProperty(const std::string& selector, const std::string& key, const ASTNode* value) {
    auto it = m_symbols.find(selector);
    if (it != m_symbols.end()) {
        it->second.properties[key] = value;
    }
}

} // namespace CHTL
