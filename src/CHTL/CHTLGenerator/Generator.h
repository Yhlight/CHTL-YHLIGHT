#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include "ExpressionEvaluator.h"
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
    void visit(const StyleNode* node);

    void collectStyles(const ASTNode* node);

    const ProgramNode& m_program;
    std::vector<std::string> m_global_styles;
    std::stringstream m_output;
    int m_indent_level = 0;
    ExpressionEvaluator m_evaluator;

    void indent();
};

} // namespace CHTL
