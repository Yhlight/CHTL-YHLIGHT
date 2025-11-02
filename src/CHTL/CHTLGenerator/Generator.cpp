#include "Generator.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/OriginNode.h"
#include "CHTL/CHTLNode/NamespaceNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"

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
        case ASTNodeType::Namespace:
            visitNamespaceNode(static_cast<NamespaceNode*>(node));
            break;
        case ASTNodeType::StyleBlock:
            // Handled by visitElementNode
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

    std::stringstream style_ss;
    for (const auto& child : node->children) {
        if (child->getType() == ASTNodeType::StyleBlock) {
            StyleNode* styleNode = static_cast<StyleNode*>(child.get());
            for (const auto& prop : styleNode->properties) {
                style_ss << prop->name << ": " << prop->value.value_or("") << ";";
            }
        }
    }
    if (style_ss.tellp() > 0) {
        m_output << " style=\"" << style_ss.str() << "\"";
    }

    m_output << ">" << std::endl;

    m_indent += 2;
    for (auto& child : node->children) {
        if (child->getType() != ASTNodeType::StyleBlock) {
            visit(child.get());
        }
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

void Generator::visitNamespaceNode(NamespaceNode* node) {
    for (auto& child : node->children) {
        visit(child.get());
    }
}

} // namespace CHTL
