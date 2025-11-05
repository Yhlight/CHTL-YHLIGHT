#pragma once

#include "../CHTL/CHTLNode/ASTNode.h"
#include <string>
#include <unordered_map>

namespace CHTL {

class SymbolTable {
public:
    void insert(const std::string& name, ASTNode* node);
    ASTNode* lookup(const std::string& name);

private:
    std::unordered_map<std::string, ASTNode*> table_;
};

} // namespace CHTL
