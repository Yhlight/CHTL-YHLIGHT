#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include "SymbolTable.h"
#include <vector>
#include <memory>

namespace CHTL {

class ProgramNode;
class TemplateNode;
class StyleNode;
class ElementNode;
class NamespaceNode;

class Analyser {
public:
    Analyser(ASTNode& root);
    void analyse();

private:
    void collect(std::vector<std::unique_ptr<ASTNode>>& nodes);
    void resolve(ASTNode* node);
    void resolve(StyleNode* node);

    ASTNode& m_root;
    SymbolTable m_symbolTable;
    std::vector<std::unique_ptr<ASTNode>> m_ownedTemplates;
};

} // namespace CHTL
