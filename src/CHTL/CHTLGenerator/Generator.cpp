#include "Generator.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/ElementNode.h"
#include "../CHTLNode/TextNode.h"
#include "../CHTLNode/StyleNode.h"
#include "../CHTLNode/ScriptNode.h"

namespace CHTL {

Generator::Generator() {}

std::string Generator::generate(ASTNode* root) {
    visit(root);
    return output_.str();
}

void Generator::visit(ASTNode* node) {
    switch (node->getType()) {
        case ASTNodeType::Program:
            visitProgramNode(node);
            break;
        case ASTNodeType::Element:
            visitElementNode(node);
            break;
        case ASTNodeType::Text:
            visitTextNode(node);
            break;
        case ASTNodeType::Style:
            visitStyleNode(node);
            break;
        case ASTNodeType::Script:
            visitScriptNode(node);
            break;
    }
}

void Generator::visitProgramNode(ASTNode* node) {
    for (const auto& child : node->children) {
        visit(child.get());
    }
}

void Generator::visitElementNode(ASTNode* node) {
    auto element = static_cast<ElementNode*>(node);
    output_ << "<" << element->tagName;

    for (const auto& attr : element->attributes) {
        output_ << " " << attr.first << "=\"" << attr.second << "\"";
    }

    std::string style;
    for (const auto& child : element->children) {
        if (child->getType() == ASTNodeType::Style) {
            style += static_cast<StyleNode*>(child.get())->content;
        }
    }

    if (!style.empty()) {
        output_ << " style=\"" << style << "\"";
    }

    output_ << ">";

    for (const auto& child : element->children) {
        if (child->getType() != ASTNodeType::Style && child->getType() != ASTNodeType::Script) {
            visit(child.get());
        }
    }

    output_ << "</" << element->tagName << ">";
}

void Generator::visitTextNode(ASTNode* node) {
    auto text = static_cast<TextNode*>(node);
    output_ << text->content;
}

void Generator::visitStyleNode(ASTNode* node) {
}

void Generator::visitScriptNode(ASTNode* node) {
}

} // namespace CHTL
