#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <variant>

namespace CHTL {

// Represents a symbol in the table, which is an element and its properties.
struct Symbol {
    std::variant<const ElementNode*, const OriginNode*> node;
    std::unordered_map<std::string, const ASTNode*> properties;
};

class SymbolTable {
public:
    SymbolTable() = default;

    void pushNamespace(const std::string& ns);
    void popNamespace();

    void insert(const std::string& selector, const ElementNode* node);
    void insert(const std::string& alias, const OriginNode* node);
    const Symbol* find(const std::string& selector) const;
    void addProperty(const std::string& selector, const std::string& key, const ASTNode* value);

private:
    std::string getNamespacedSelector(const std::string& selector) const;

    std::unordered_map<std::string, Symbol> m_symbols;
    std::vector<std::string> m_namespaceStack;
};

} // namespace CHTL
