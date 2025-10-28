#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include <string>
#include <sstream>

namespace CHTL {

class Generator {
public:
    explicit Generator(const ProgramNode& program);
    std::string generate();

private:
    void visit(const ASTNode* node);
    void visit(const ElementNode* node);
    void visit(const TextNode* node);
    void visit(const ProgramNode* node);

    const ProgramNode& m_program;
    std::stringstream m_output;
    int m_indent_level = 0;

    void indent();
};

} // namespace CHTL
