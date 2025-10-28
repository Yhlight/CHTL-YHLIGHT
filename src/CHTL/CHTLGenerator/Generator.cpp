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
        case NodeType::Style:
             // Style nodes are handled within element nodes, so we don't visit them directly.
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

    bool has_class = false;
    bool has_id = false;
    for (const auto& attr : node->attributes) {
        if (attr.key == "class") has_class = true;
        if (attr.key == "id") has_id = true;
        m_output << " " << attr.key << "=\"" << attr.value << "\"";
    }

    if (!has_class && !node->auto_classes.empty()) {
        m_output << " class=\"" << node->auto_classes[0] << "\"";
    }
    if (!has_id && !node->auto_ids.empty()) {
        m_output << " id=\"" << node->auto_ids[0] << "\"";
    }

    if (node->style && !node->style->properties.empty()) {
        m_output << " style=\"";
        for (size_t i = 0; i < node->style->properties.size(); ++i) {
            const auto& prop = node->style->properties[i];
            m_output << prop.key << ": " << prop.value << ";";
            if (i < node->style->properties.size() - 1) {
                m_output << " ";
            }
        }
        m_output << "\"";
    }


    // CHTL does not use self-closing tags for elements that can have content,
    // to align with HTML semantics (e.g., div, p, etc.).
    // We will always generate a closing tag.
    if (node->children.empty()) {
        m_output << "></" << node->tagName << ">\n";
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
