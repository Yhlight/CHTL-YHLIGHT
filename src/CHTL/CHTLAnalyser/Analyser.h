#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include "SymbolTable.h"
#include "CHTL/CHTLImporter/Importer.h"
#include <memory>
#include <unordered_set>

namespace CHTL {

class Analyser {
public:
    explicit Analyser(ProgramNode& program, std::string filePath);
    void analyse();

private:
    void visit(ASTNode* node);
    void visit(ElementNode* node);
    void visit(ProgramNode* node);
    void visit(TemplateNode* node);
    void visit(ImportNode* node);
    void visit(NamespaceNode* node);

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
    Importer m_importer;
    std::string m_filePath;
    std::unordered_set<std::string> m_import_stack;
};

} // namespace CHTL
