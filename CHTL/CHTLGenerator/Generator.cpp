#include "Generator.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/CommentNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/ScriptNode.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/StyleContentNode.h"
#include <iostream>

Generator::Generator(AstNodeList ast) : ast(std::move(ast)) {}

std::string Generator::generate() {
    for (const auto& node : ast) {
        visit(node.get());
    }
    return html_output;
}

void Generator::visit(BaseNode* node) {
    if (auto elementNode = dynamic_cast<ElementNode*>(node)) {
        visitElement(elementNode);
    } else if (auto textNode = dynamic_cast<TextNode*>(node)) {
        visitText(textNode);
    } else if (auto commentNode = dynamic_cast<CommentNode*>(node)) {
        visitComment(commentNode);
    } else if (auto styleNode = dynamic_cast<StyleNode*>(node)) {
        visitStyle(styleNode);
    } else if (auto scriptNode = dynamic_cast<ScriptNode*>(node)) {
        visitScript(scriptNode);
    } else if (auto templateNode = dynamic_cast<TemplateNode*>(node)) {
        visitTemplate(templateNode);
    }
}

void Generator::visitElement(ElementNode* node) {
    html_output += "<" + node->tag_name;
    for (const auto& attr : node->attributes) {
        html_output += " " + attr.first + "=\"" + attr.second + "\"";
    }
    html_output += ">";

    for (const auto& child : node->children) {
        visit(child.get());
    }

    html_output += "</" + node->tag_name + ">";
}

void Generator::visitText(TextNode* node) {
    html_output += node->text;
}

void Generator::visitComment(CommentNode* node) {
    html_output += "<!--" + node->comment + " -->";
}

void Generator::visitStyle(StyleNode* node) {
    html_output += "<style>";
    for (const auto& content_node : node->content) {
        if (auto rawNode = dynamic_cast<RawStyleContentNode*>(content_node.get())) {
            html_output += rawNode->raw_css;
        } else if (auto directiveNode = dynamic_cast<StyleDirectiveNode*>(content_node.get())) {
            if (style_templates.count(directiveNode->template_name)) {
                html_output += style_templates[directiveNode->template_name];
            }
        }
    }
    html_output += "</style>";
}

void Generator::visitScript(ScriptNode* node) {
    html_output += "<script>" + node->content + "</script>";
}

void Generator::visitTemplate(TemplateNode* node) {
    if (node->type == "@Style") {
        std::string style_content;
        for (const auto& child : node->body) {
            if (auto textNode = dynamic_cast<TextNode*>(child.get())) {
                style_content += textNode->text;
            }
        }
        style_templates[node->name] = style_content;
    }
}
