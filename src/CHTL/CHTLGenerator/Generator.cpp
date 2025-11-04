#include "Generator.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/OriginNode.h"
#include "CHTL/CHTLNode/NamespaceNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include <vector>

namespace CHTL {

Generator::Generator(ASTNode& root) : m_root(root) {}

std::string Generator::generate(bool default_struct, bool inline_output) {
    m_default_struct = default_struct;
    m_inline_output = inline_output;

    if (m_default_struct) {
        m_output << "<!DOCTYPE html>\n<html>\n<head>\n</head>\n<body>\n";
    }

    visit(&m_root);

    if (m_default_struct) {
        m_output << "</body>\n</html>\n";
    }
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
    std::stringstream css_output;
    std::stringstream js_output;

    if (m_inline_output) {
        // First pass: collect all CSS and JS
        for (auto& child : node->children) {
            if (child->getType() == ASTNodeType::Element) {
                // In inline mode, we need to traverse the whole tree to find styles and scripts
                std::vector<ASTNode*> nodes_to_visit;
                nodes_to_visit.push_back(child.get());
                while(!nodes_to_visit.empty()){
                    ASTNode* current = nodes_to_visit.back();
                    nodes_to_visit.pop_back();

                    if(current->getType() == ASTNodeType::Element){
                       ElementNode* element = static_cast<ElementNode*>(current);
                        for(auto& el_child : element->children){
                            if(el_child->getType() == ASTNodeType::StyleBlock){
                                StyleNode* styleNode = static_cast<StyleNode*>(el_child.get());
                                for (const auto& prop : styleNode->properties) {
                                    if (prop->getType() == ASTNodeType::StyleProperty) {
                                        StylePropertyNode* propNode = static_cast<StylePropertyNode*>(prop.get());
                                        css_output << propNode->name << ": " << propNode->value.value_or("") << ";";
                                    }
                                }
                            }
                            else {
                                nodes_to_visit.push_back(el_child.get());
                            }
                        }
                    } else if (child->getType() == ASTNodeType::Origin) {
                        OriginNode* originNode = static_cast<OriginNode*>(child.get());
                        if (originNode->originType == "@Style") {
                            css_output << originNode->content;
                        } else if (originNode->originType == "@JavaScript") {
                            js_output << originNode->content;
                        }
                    }
                }
            }
        }
    }


    if (m_inline_output && css_output.tellp() > 0) {
        m_output << "<style>\n" << css_output.str() << "\n</style>\n";
    }

    for (auto& child : node->children) {
        visit(child.get());
    }

    if (m_inline_output && js_output.tellp() > 0) {
        m_output << "<script>\n" << js_output.str() << "\n</script>\n";
    }
}

void Generator::visitElementNode(ElementNode* node) {
    m_output << std::string(m_indent, ' ') << "<" << node->tagName;
    for (const auto& attr : node->attributes) {
        m_output << " " << attr.first << "=" << attr.second;
    }

    if (!m_inline_output) {
        std::stringstream style_ss;
        for (const auto& child : node->children) {
            if (child->getType() == ASTNodeType::StyleBlock) {
                StyleNode* styleNode = static_cast<StyleNode*>(child.get());
                for (const auto& prop : styleNode->properties) {
                    if (prop->getType() == ASTNodeType::StyleProperty) {
                        StylePropertyNode* propNode = static_cast<StylePropertyNode*>(prop.get());
                        style_ss << propNode->name << ": " << propNode->value.value_or("") << ";";
                    }
                }
            }
        }
        if (style_ss.tellp() > 0) {
            m_output << " style=\"" << style_ss.str() << "\"";
        }
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
    if (!m_inline_output || (node->originType != "@Style" && node->originType != "@JavaScript")) {
        m_output << node->content;
    }
}

void Generator::visitNamespaceNode(NamespaceNode* node) {
    for (auto& child : node->children) {
        visit(child.get());
    }
}

} // namespace CHTL
