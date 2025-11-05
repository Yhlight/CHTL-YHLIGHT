#include "Generator.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/ElementNode.h"
#include "../CHTLNode/TextNode.h"
#include "../CHTLNode/StyleNode.h"
#include "../CHTLNode/ScriptNode.h"
#include "../CHTLNode/StylePropertyNode.h"
#include "../CHTLNode/ValueNode/ValueNode.h"
#include "../CHTLNode/ValueNode/LiteralValueNode.h"

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
        case ASTNodeType::StyleProperty:
            visitStylePropertyNode(node);
            break;
        case ASTNodeType::Template:
            // Templates are handled by the analyser, so the generator should ignore them.
            break;
        case ASTNodeType::TemplateUsage:
            // Template usages are handled by the analyser, so the generator should ignore them.
            break;
        case ASTNodeType::Origin:
            visitOriginNode(node);
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

    std::stringstream styleStream;
    for (const auto& child : element->children) {
        if (child->getType() == ASTNodeType::Style) {
            for (const auto& styleChild : child->children) {
                if (styleChild->getType() == ASTNodeType::StyleProperty) {
                    auto prop = static_cast<StylePropertyNode*>(styleChild.get());
                    if (prop->value->valueType == ValueNodeType::Literal) {
                        auto literalValue = static_cast<LiteralValueNode*>(prop->value.get());
                        styleStream << prop->name << ":" << literalValue->value << ";";
                    }
                }
            }
        }
    }

    if (styleStream.tellp() > 0) {
        output_ << " style=\"" << styleStream.str() << "\"";
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

void Generator::visitStylePropertyNode(ASTNode* node) {
}

void Generator::visitScriptNode(ASTNode* node) {
}

void Generator::visitOriginNode(ASTNode* node) {
    auto origin = static_cast<OriginNode*>(node);
    output_ << origin->content;
}

} // namespace CHTL
