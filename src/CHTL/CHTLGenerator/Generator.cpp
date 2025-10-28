#include "Generator.h"

namespace CHTL {

Generator::Generator(const ProgramNode& program) : m_program(program) {}

std::string Generator::generate() {
    visit(&m_program);
    return m_output.str();
}

void Generator::visit(const ASTNode* node) {
    if (!node) return;

    switch (node->getType()) {
        case NodeType::Program:
            visit(static_cast<const ProgramNode*>(node));
            break;
        case NodeType::Element:
            visit(static_cast<const ElementNode*>(node));
            break;
        case NodeType::Text:
            visit(static_cast<const TextNode*>(node));
            break;
    }
}

void Generator::visit(const ProgramNode* node) {
    for (const auto& child : node->children) {
        visit(child.get());
    }
}

void Generator::visit(const ElementNode* node) {
    indent();
    m_output << "<" << node->tagName;

    for (const auto& attr : node->attributes) {
        m_output << " " << attr.key << "=\"" << attr.value << "\"";
    }

    if (node->children.empty()) {
        m_output << " />\n";
        return;
    }

    m_output << ">\n";
    m_indent_level++;

    for (const auto& child : node->children) {
        visit(child.get());
    }

    m_indent_level--;
    indent();
    m_output << "</" << node->tagName << ">\n";
}

void Generator::visit(const TextNode* node) {
    indent();
    m_output << node->content << "\n";
}

void Generator::indent() {
    for (int i = 0; i < m_indent_level; ++i) {
        m_output << "  "; // 2 spaces for indentation
    }
}

} // namespace CHTL
