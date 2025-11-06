#include "Generator.h"

std::string Generator::generate(const ProgramNode& program) {
    visit(&program);
    return output_.str();
}

void Generator::visit(const ASTNode* node) {
    if (node == nullptr) return;

    switch (node->getType()) {
        case ASTNodeType::Program:
            visitProgramNode(static_cast<const ProgramNode*>(node));
            break;
        case ASTNodeType::Element:
            visitElementNode(static_cast<const ElementNode*>(node));
            break;
        case ASTNodeType::Text:
            visitTextNode(static_cast<const TextNode*>(node));
            break;
    }
}

void Generator::visitProgramNode(const ProgramNode* node) {
    for (const auto& child : node->getChildren()) {
        visit(child.get());
    }
}

void Generator::visitElementNode(const ElementNode* node) {
    output_ << "<" << node->getTagName();

    for (const auto& attr : node->getAttributes()) {
        output_ << " " << attr.first << "=\"" << attr.second << "\"";
    }

    output_ << ">";

    for (const auto& child : node->getChildren()) {
        visit(child.get());
    }

    output_ << "</" << node->getTagName() << ">";
}

void Generator::visitTextNode(const TextNode* node) {
    output_ << node->getText();
}
