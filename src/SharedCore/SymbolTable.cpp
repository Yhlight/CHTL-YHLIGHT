#include "SymbolTable.h"
#include "CHTL/CHTLNode/TemplateNode.h"

void SymbolTable::insert(const std::string& name, TemplateNode* node) {
    table_[name] = node;
}

TemplateNode* SymbolTable::lookup(const std::string& name) const {
    auto it = table_.find(name);
    if (it != table_.end()) {
        return it->second;
    }
    return nullptr;
}
