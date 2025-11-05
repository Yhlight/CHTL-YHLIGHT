#include "SymbolTable.h"

namespace CHTL {

void SymbolTable::insert(const std::string& name, ASTNode* node) {
    table_[name] = node;
}

ASTNode* SymbolTable::lookup(const std::string& name) {
    if (table_.count(name)) {
        return table_[name];
    }
    return nullptr;
}

} // namespace CHTL
