#pragma once

#include "CHTLNode/ASTNode.h"
#include "CHTLNode/ProgramNode.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include <string>
#include <sstream>

class Generator {
public:
    std::string generate(const ProgramNode& program);

private:
    void visit(const ASTNode* node);
    void visitProgramNode(const ProgramNode* node);
    void visitElementNode(const ElementNode* node);
    void visitTextNode(const TextNode* node);

    std::stringstream output_;
};
