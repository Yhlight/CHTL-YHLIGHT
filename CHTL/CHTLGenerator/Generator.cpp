#include "Generator.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/CommentNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/ScriptNode.h"
#include "CHTLNode/TemplateNode.h"
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
    std::string content = node->content;
    size_t pos = content.find("@Style ");
    if (pos != std::string::npos) {
        size_t end_pos = content.find(";", pos);
        std::string template_name = content.substr(pos + 7, end_pos - (pos + 7));
        if (style_templates.count(template_name)) {
            content.replace(pos, end_pos - pos + 1, style_templates[template_name]);
        }
    }
    html_output += "<style>" + content + "</style>";
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
