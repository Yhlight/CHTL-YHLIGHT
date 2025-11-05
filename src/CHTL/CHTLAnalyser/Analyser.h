#pragma once

#include "../CHTLNode/ASTNode.h"
#include "../../SharedCore/SymbolTable.h"

namespace CHTL {

class Analyser {
public:
    Analyser(SymbolTable& symbolTable);
    void analyse(ASTNode* root);

private:
    void visit(ASTNode* node);
    void visitProgramNode(ASTNode* node);
    void visitStyleNode(ASTNode* node);
    void visitElementNode(ASTNode* node);

    SymbolTable& symbolTable_;
    std::vector<std::unique_ptr<TemplateNode>> ownedTemplates_;
};

} // namespace CHTL
