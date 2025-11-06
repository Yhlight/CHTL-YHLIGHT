#pragma once

#include "SymbolTable.h"
#include "CHTLNode/ASTNode.h"
#include "CHTLNode/ProgramNode.h"
#include <vector>
#include <memory>

class TemplateNode;

class Analyser {
public:
    explicit Analyser(SymbolTable& symbolTable);

    void analyse(ProgramNode& program);

private:
    void collectTemplates(ASTNode* node);
    void resolveUsages(ASTNode* node);

    SymbolTable& symbolTable_;
    std::vector<std::unique_ptr<ASTNode>> ownedTemplates_;
};
