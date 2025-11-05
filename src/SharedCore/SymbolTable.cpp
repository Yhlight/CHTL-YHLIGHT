#include "SymbolTable.h"

namespace CHTL {

void SymbolTable::insert(const std::string& name, TemplateNode* node) {
    table_[name] = node;
}

TemplateNode* SymbolTable::lookup(const std::string& name) {
    if (table_.count(name)) {
        return table_[name];
    }
    return nullptr;
}

} // namespace CHTL
