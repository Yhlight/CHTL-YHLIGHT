#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include <string>
#include <sstream>

namespace CHTL {

class ProgramNode;
class ElementNode;
class TextNode;
class OriginNode;
class NamespaceNode;
class StyleNode;

class Generator {
public:
    Generator(ASTNode& root);
    std::string generate(bool default_struct = false, bool inline_output = false);

private:
    ASTNode& m_root;
    bool m_default_struct = false;
    bool m_inline_output = false;
    std::stringstream m_output;
    int m_indent = 0;

    void visit(ASTNode* node);
    void visitProgramNode(ProgramNode* node);
    void visitElementNode(ElementNode* node);
    void visitTextNode(TextNode* node);
    void visitOriginNode(OriginNode* node);
    void visitNamespaceNode(NamespaceNode* node);
};

} // namespace CHTL
