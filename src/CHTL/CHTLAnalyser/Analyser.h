#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include "SymbolTable.h"
#include <memory>

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

    void resolve(ASTNode* node);
    void resolve(ElementNode* node);
    void resolve(ProgramNode* node);
    void resolve(StyleNode* node);
    void resolve(StyleProperty& prop);
    void resolve(std::unique_ptr<ASTNode>& node);
    void resolve(TemplateUsageNode* node);

    ProgramNode& m_program;
    SymbolTable m_symbol_table;
    std::unordered_map<std::string, const TemplateNode*> m_templates;
};

} // namespace CHTL
