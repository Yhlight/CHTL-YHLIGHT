#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace CHTL {

// Represents a symbol in the table, which is an element and its properties.
struct Symbol {
    const ElementNode* node;
    std::unordered_map<std::string, const ASTNode*> properties;
};

class SymbolTable {
public:
    SymbolTable() = default;

    void insert(const std::string& selector, const ElementNode* node);
    const Symbol* find(const std::string& selector) const;
    void addProperty(const std::string& selector, const std::string& key, const ASTNode* value);

private:
    std::unordered_map<std::string, Symbol> m_symbols;
};

} // namespace CHTL
