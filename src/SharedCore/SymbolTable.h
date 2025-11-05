#pragma once

#include "../CHTL/CHTLNode/TemplateNode.h"
#include <string>
#include <unordered_map>

namespace CHTL {

class SymbolTable {
public:
    void insert(const std::string& name, TemplateNode* node);
    TemplateNode* lookup(const std::string& name);

private:
    std::unordered_map<std::string, TemplateNode*> table_;
};

} // namespace CHTL
