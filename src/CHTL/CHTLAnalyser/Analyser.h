#pragma once

#include "../CHTLNode/ASTNode.h"
#include "../../SharedCore/SymbolTable.h"
#include "../CHTLImporter/Importer.h"

namespace CHTL {

// Forward declarations
class TemplateNode;
class ImportNode;

class Analyser {
public:
    Analyser(SymbolTable& symbolTable);
    void analyse(ASTNode* root);

private:
    void visit(ASTNode* node);
    void visitProgramNode(ASTNode* node);
    void visitStyleNode(ASTNode* node);
    void visitElementNode(ASTNode* node);
    void visitStylePropertyNode(ASTNode* node);
    void visitImportNode(ASTNode* node);

    SymbolTable& symbolTable_;
    std::vector<std::unique_ptr<TemplateNode>> ownedTemplates_;
    Importer importer_;
};

} // namespace CHTL
