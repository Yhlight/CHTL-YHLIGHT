#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include <string>
#include <sstream>

namespace CHTL {

class ProgramNode;
class ElementNode;
class TextNode;
class OriginNode;

class Generator {
public:
    Generator(ASTNode& root);
    std::string generate();

private:
    ASTNode& m_root;
    std::stringstream m_output;
    int m_indent = 0;

    void visit(ASTNode* node);
    void visitProgramNode(ProgramNode* node);
    void visitElementNode(ElementNode* node);
    void visitTextNode(TextNode* node);
    void visitOriginNode(OriginNode* node);
};

} // namespace CHTL
