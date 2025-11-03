#include "Generator.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/CommentNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/ScriptNode.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/StyleContentNode.h"
#include "CHTLNode/ElementDirectiveNode.h"
#include "CHTLNode/CustomNode.h"
#include <iostream>

Generator::Generator(AstNodeList ast) : ast(std::move(ast)) {}

std::string Generator::generate() {
    for (const auto& node : ast) {
        visit(node.get());
    }

    std::string final_output;
    if (html_output.find("<body") != std::string::npos) {
        final_output = "<html><head><style>" + css_output + "</style></head>" + html_output + "<script>" + js_output + "</script></html>";
    } else {
        final_output = "<html><head><style>" + css_output + "</style></head><body>" + html_output + "<script>" + js_output + "</script></body></html>";
    }
    return final_output;
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
    } else if (auto elementDirectiveNode = dynamic_cast<ElementDirectiveNode*>(node)) {
        visitElementDirective(elementDirectiveNode);
    } else if (auto customNode = dynamic_cast<CustomNode*>(node)) {
        visitCustom(customNode);
    } else if (auto originNode = dynamic_cast<OriginNode*>(node)) {
        visitOrigin(originNode);
    } else if (auto importNode = dynamic_cast<ImportNode*>(node)) {
        visitImport(importNode);
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
    for (const auto& content_node : node->content) {
        if (auto rawNode = dynamic_cast<RawStyleContentNode*>(content_node.get())) {
            std::string css = rawNode->raw_css;
            for (const auto& var_template : variable_templates) {
                std::string template_name = var_template.first;
                for (const auto& var : var_template.second) {
                    std::string var_name = var.first;
                    std::string var_value = var.second;
                    std::string search_str = template_name + "(" + var_name + ")";
                    size_t pos = css.find(search_str);
                    while (pos != std::string::npos) {
                        css.replace(pos, search_str.length(), "\"" + var_value + "\"");
                        pos = css.find(search_str, pos + var_value.length() + 2);
                    }
                }
            }
            css_output += css;
        } else if (auto directiveNode = dynamic_cast<StyleDirectiveNode*>(content_node.get())) {
            if (style_templates.count(directiveNode->template_name)) {
                css_output += style_templates[directiveNode->template_name];
            } else if (custom_style_templates.count(directiveNode->template_name)) {
                CustomNode* customNode = custom_style_templates[directiveNode->template_name];
                for (const auto& prop : customNode->valueless_properties) {
                    if (directiveNode->properties.count(prop)) {
                        css_output += prop + ": " + directiveNode->properties[prop] + ";";
                    }
                }
            }
        }
    }
}

void Generator::visitScript(ScriptNode* node) {
    js_output += node->content;
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
    } else if (node->type == "@Element") {
        element_templates[node->name] = node;
    } else if (node->type == "@Var") {
        variable_templates[node->name] = node->variables;
    }
}

void Generator::visitElementDirective(ElementDirectiveNode* node) {
    if (element_templates.count(node->template_name)) {
        TemplateNode* templateNode = element_templates[node->template_name];
        for (const auto& child : templateNode->body) {
            visit(child.get());
        }
    }
}

void Generator::visitCustom(CustomNode* node) {
    if (node->type == "@Style") {
        custom_style_templates[node->name] = node;
    }
}

void Generator::visitOrigin(OriginNode* node) {
    if (node->type == "@Html") {
        html_output += node->content;
    } else if (node->type == "@Style") {
        css_output += node->content;
    } else if (node->type == "@JavaScript") {
        js_output += node->content;
    }
}

void Generator::visitImport(ImportNode* node) {
    // For now, this is a placeholder.
    // In the future, this will handle file loading and parsing.
}
