#pragma once

#include <string>
#include <unordered_map>
#include <memory>

class TemplateNode; // Forward declaration

class SymbolTable {
public:
    void insert(const std::string& name, TemplateNode* node);
    TemplateNode* lookup(const std::string& name) const;

private:
    std::unordered_map<std::string, TemplateNode*> table_;
};
