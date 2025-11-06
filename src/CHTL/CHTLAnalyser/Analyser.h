#pragma once

#include "SymbolTable.h"
#include "CHTLNode/ASTNode.h"
#include "CHTLNode/ProgramNode.h"
#include "CHTLImporter/Importer.h"
#include <vector>
#include <memory>
#include <filesystem>

class TemplateNode;

class Analyser {
public:
    explicit Analyser(SymbolTable& symbolTable);

    void analyse(ProgramNode& program);

private:
    void resolveImports(ASTNode* node);
    void collectTemplates(ASTNode* node);
    void resolveUsages(ASTNode* node);

    SymbolTable& symbolTable_;
    Importer importer_;
    std::vector<std::unique_ptr<ASTNode>> ownedTemplates_;
    std::vector<std::filesystem::path> importStack_;
};
