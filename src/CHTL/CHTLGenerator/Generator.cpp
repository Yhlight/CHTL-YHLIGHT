#include "Generator.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/OriginNode.h"

namespace CHTL {

Generator::Generator(ASTNode& root) : m_root(root) {}

std::string Generator::generate() {
    visit(&m_root);
    return m_output.str();
}

void Generator::visit(ASTNode* node) {
    switch (node->getType()) {
        case ASTNodeType::Program:
            visitProgramNode(static_cast<ProgramNode*>(node));
            break;
        case ASTNodeType::Element:
            visitElementNode(static_cast<ElementNode*>(node));
            break;
        case ASTNodeType::Text:
            visitTextNode(static_cast<TextNode*>(node));
            break;
        case ASTNodeType::Origin:
            visitOriginNode(static_cast<OriginNode*>(node));
            break;
        default:
            break;
    }
}

void Generator::visitProgramNode(ProgramNode* node) {
    for (auto& child : node->children) {
        visit(child.get());
    }
}

void Generator::visitElementNode(ElementNode* node) {
    m_output << std::string(m_indent, ' ') << "<" << node->tagName;
    for (const auto& attr : node->attributes) {
        m_output << " " << attr.first << "=" << attr.second;
    }
    m_output << ">" << std::endl;

    m_indent += 2;
    for (auto& child : node->children) {
        visit(child.get());
    }
    m_indent -= 2;

    m_output << std::string(m_indent, ' ') << "</" << node->tagName << ">" << std::endl;
}

void Generator::visitTextNode(TextNode* node) {
    m_output << std::string(m_indent, ' ') << node->content << std::endl;
}

void Generator::visitOriginNode(OriginNode* node) {
    m_output << node->content;
}

} // namespace CHTL
