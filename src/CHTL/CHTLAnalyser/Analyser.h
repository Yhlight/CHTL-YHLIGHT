#pragma once

#include "../CHTLNode/ASTNode.h"
#include "../../SharedCore/SymbolTable.h"
#include "../CHTLImporter/Importer.h"
#include "../CHTLNode/OriginNode.h"

namespace CHTL {

// Forward declarations
class TemplateNode;
class ImportNode;
class OriginNode;

class Analyser {
public:
    Analyser(SymbolTable& symbolTable);
    ~Analyser();
    void analyse(ASTNode* root);

private:
    void visit(ASTNode* node);
    void visitProgramNode(ASTNode* node);
    void visitStyleNode(ASTNode* node);
    void visitElementNode(ASTNode* node);
    void visitStylePropertyNode(ASTNode* node);
    void visitImportNode(ASTNode* node);
    void visitOriginNode(ASTNode* node);

    SymbolTable& symbolTable_;
    std::vector<std::unique_ptr<TemplateNode>> ownedTemplates_;
    Importer importer_;
};

} // namespace CHTL
