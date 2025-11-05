#include "Generator.h"
#include "ProgramNode.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"
#include "StylePropertyNode.h"
#include "StyleRuleNode.h"
#include "ScriptNode.h"
#include "TemplateNode.h"
#include "ElementDirectiveNode.h"
#include "StyleDirectiveNode.h"

Generator::Generator(const BaseNode& root) : root(root) {}

std::string Generator::generate(bool full_document) {
    visit(&root);

    if (full_document) {
        std::stringstream final_output;
        final_output << "<html><head><style>" << css_output.str() << "</style></head><body>";
        final_output << html_output.str();
        final_output << "<script>" << js_output.str() << "</script>";
        final_output << "</body></html>";
        return final_output.str();
    } else {
        return html_output.str();
    }
}

void Generator::visit(const BaseNode* node) {
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
            // Style nodes are handled by the parent element
            break;
        case NodeType::Script:
            visit(static_cast<const ScriptNode*>(node));
            break;
        case NodeType::Template:
            visit(static_cast<const TemplateNode*>(node));
            break;
        case NodeType::ElementDirective:
            visit(static_cast<const ElementDirectiveNode*>(node));
            break;
        case NodeType::StyleDirective:
            visit(static_cast<const StyleDirectiveNode*>(node));
            break;
    }
}

void Generator::visit(const ProgramNode* node) {
    for (const auto& child : node->children) {
        visit(child.get());
    }
}

void Generator::visit(const ElementNode* node) {
    html_output << "<" << node->tag_name;
    for (const auto& attr : node->attributes) {
        html_output << " " << attr.first << "=\"" << attr.second << "\"";
    }

    std::stringstream style_attr;
    for (const auto& child : node->children) {
        if (child && child->getType() == NodeType::Style) {
            auto style_node = static_cast<const StyleNode*>(child.get());
            for (const auto& style_content : style_node->children) {
                if (style_content->getStyleContentType() == StyleContentType::Property) {
                    auto prop_node = static_cast<const StylePropertyNode*>(style_content.get());
                    std::string value = prop_node->value;
                    size_t start_pos = value.find('(');
                    if (start_pos != std::string::npos) {
                        std::string template_name = value.substr(0, start_pos);
                        std::string var_name = value.substr(start_pos + 1, value.length() - start_pos - 2);
                        auto it = var_templates.find(template_name);
                        if (it != var_templates.end()) {
                            auto var_it = it->second->variables.find(var_name);
                            if (var_it != it->second->variables.end()) {
                                value = var_it->second;
                            }
                        }
                    }
                    style_attr << prop_node->key << ":" << value << ";";
                } else if (style_content->getStyleContentType() == StyleContentType::Rule) {
                    auto rule_node = static_cast<const StyleRuleNode*>(style_content.get());
                    css_output << "." << rule_node->selector << "{";
                    for (const auto& prop : rule_node->children) {
                        auto prop_node = static_cast<const StylePropertyNode*>(prop.get());
                        css_output << prop_node->key << ":" << prop_node->value << ";";
                    }
                    css_output << "}";
                    html_output << " class=\"" << rule_node->selector << "\"";
                } else if (style_content->getStyleContentType() == StyleContentType::Directive) {
                    auto directive_node = static_cast<const StyleDirectiveNode*>(style_content.get());
                    auto it = style_templates.find(directive_node->name);
                    if (it != style_templates.end()) {
                        for (const auto& prop : it->second->children) {
                            auto prop_node = static_cast<const StylePropertyNode*>(prop.get());
                            style_attr << prop_node->key << ":" << prop_node->value << ";";
                        }
                    }
                }
            }
        }
    }

    if (style_attr.str().length() > 0) {
        html_output << " style=\"" << style_attr.str() << "\"";
    }

    html_output << ">";
    for (const auto& child : node->children) {
        if (child && child->getType() != NodeType::Style) {
            visit(child.get());
        }
    }
    html_output << "</" << node->tag_name << ">";
}

void Generator::visit(const TextNode* node) {
    html_output << node->text;
}

void Generator::visit(const ScriptNode* node) {
    js_output << node->content;
}

void Generator::visit(const TemplateNode* node) {
    if (node->type == TemplateType::Element) {
        element_templates[node->name] = node;
    } else if (node->type == TemplateType::Style) {
        style_templates[node->name] = node;
    } else if (node->type == TemplateType::Var) {
        var_templates[node->name] = node;
    }
}

void Generator::visit(const ElementDirectiveNode* node) {
    auto it = element_templates.find(node->name);
    if (it != element_templates.end()) {
        for (const auto& child : it->second->children) {
            visit(child.get());
        }
    }
}

void Generator::visit(const StyleDirectiveNode* node) {
    // This is handled by the parent element
}
