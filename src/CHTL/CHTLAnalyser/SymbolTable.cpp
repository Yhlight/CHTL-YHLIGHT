#include "SymbolTable.h"
#include <sstream>

namespace CHTL {

void SymbolTable::pushNamespace(const std::string& ns) {
    m_namespaceStack.push_back(ns);
}

void SymbolTable::popNamespace() {
    if (!m_namespaceStack.empty()) {
        m_namespaceStack.pop_back();
    }
}

void SymbolTable::insert(const std::string& selector, const ElementNode* node) {
    m_symbols[getNamespacedSelector(selector)] = {node, {}};
}

void SymbolTable::insert(const std::string& alias, const OriginNode* node) {
    m_symbols[getNamespacedSelector(alias)] = {node, {}};
}

const Symbol* SymbolTable::find(const std::string& selector) const {
    auto it = m_symbols.find(getNamespacedSelector(selector));
    if (it != m_symbols.end()) {
        return &it->second;
    }
    return nullptr;
}

void SymbolTable::addProperty(const std::string& selector, const std::string& key, const ASTNode* value) {
    auto it = m_symbols.find(getNamespacedSelector(selector));
    if (it != m_symbols.end()) {
        it->second.properties[key] = value;
    }
}

std::string SymbolTable::getNamespacedSelector(const std::string& selector) const {
    if (m_namespaceStack.empty()) {
        return selector;
    }

    std::stringstream ss;
    for (const auto& ns : m_namespaceStack) {
        ss << ns << ".";
    }
    ss << selector;
    return ss.str();
}

} // namespace CHTL
