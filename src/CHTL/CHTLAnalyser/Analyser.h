#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include "SymbolTable.h"
#include <memory>
#include <unordered_set>

namespace CHTL {

class Analyser {
public:
    explicit Analyser(ProgramNode& program);
    void analyse();

private:
    void visit(ASTNode* node);
    void visit(ElementNode* node);
    void visit(ProgramNode* node);
    void visit(TemplateNode* node);

    void resolveInheritance(TemplateNode* node);

    void resolve(ASTNode* node);
    void resolve(ElementNode* node);
    void resolve(ProgramNode* node);
    void resolve(StyleNode* node);
    void resolve(StyleProperty& prop);
    void resolve(std::unique_ptr<ASTNode>& node);
    void resolve(TemplateUsageNode* node);

    void applySpecializations(std::vector<std::unique_ptr<ASTNode>>& elements, const TemplateUsageNode* usage_node);

    ProgramNode& m_program;
    SymbolTable m_symbol_table;
    std::unordered_map<std::string, TemplateNode*> m_templates;
    std::unordered_set<std::string> m_inheritance_stack;
};

} // namespace CHTL
