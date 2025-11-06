#pragma once

#include "CHTLNode/ASTNode.h"
#include "CHTLNode/ProgramNode.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/StylePropertyNode.h"
#include "CHTLNode/LiteralValueNode.h"
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
    void visitStyleNode(const StyleNode* node);
    void visitStylePropertyNode(const StylePropertyNode* node);


    std::stringstream output_;
    std::stringstream style_buffer_;
};
