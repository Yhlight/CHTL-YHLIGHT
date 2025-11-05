#include "Generator.h"
#include "ProgramNode.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"
#include "StylePropertyNode.h"
#include "StyleRuleNode.h"
#include "ScriptNode.h"
#include "TemplateNode.h"
#include "CustomNode.h"
#include "ElementDirectiveNode.h"
#include "StyleDirectiveNode.h"
#include "OriginNode.h"
#include "OriginDirectiveNode.h"
#include "ImportNode.h"
#include "NamespaceNode.h"
#include "ConfigurationNode.h"
#include "FileUtil.h"

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
        case NodeType::Custom:
            visit(static_cast<const CustomNode*>(node));
            break;
        case NodeType::ElementDirective:
            visit(static_cast<const ElementDirectiveNode*>(node));
            break;
        case NodeType::StyleDirective:
            visit(static_cast<const StyleDirectiveNode*>(node));
            break;
        case NodeType::Origin:
            visit(static_cast<const OriginNode*>(node));
            break;
        case NodeType::OriginDirective:
            visit(static_cast<const OriginDirectiveNode*>(node));
            break;
        case NodeType::Import:
            visit(static_cast<const ImportNode*>(node));
            break;
        case NodeType::Namespace:
            visit(static_cast<const NamespaceNode*>(node));
            break;
        case NodeType::Configuration:
            visit(static_cast<const ConfigurationNode*>(node));
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
                        auto it = var_templates[current_namespace].find(template_name);
                        if (it == var_templates[current_namespace].end()) {
                            it = var_templates[""].find(template_name);
                        }
                        if (it != var_templates[current_namespace].end()) {
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
                    if (config.auto_add_class) {
                        html_output << " class=\"" << rule_node->selector << "\"";
                    }
                } else if (style_content->getStyleContentType() == StyleContentType::Directive) {
                    style_attr << visit(static_cast<const StyleDirectiveNode*>(style_content.get()));
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

void Generator::visit(const ConfigurationNode* node) {
    for (const auto& option : node->options) {
        if (option.first == "DISABLE_STYLE_AUTO_ADD_CLASS") {
            config.auto_add_class = (option.second != "true");
        } else if (option.first == "DISABLE_STYLE_AUTO_ADD_ID") {
            config.auto_add_id = (option.second != "true");
        }
    }
}

void Generator::visit(const NamespaceNode* node) {
    std::string previous_namespace = current_namespace;
    current_namespace = node->name;
    for (const auto& child : node->children) {
        visit(child.get());
    }
    current_namespace = previous_namespace;
}

void Generator::visit(const TextNode* node) {
    html_output << node->text;
}

void Generator::visit(const ScriptNode* node) {
    js_output << node->content;
}

void Generator::visit(const TemplateNode* node) {
    if (node->type == TemplateType::Element) {
        element_templates[current_namespace][node->name] = node;
    } else if (node->type == TemplateType::Style) {
        style_templates[current_namespace][node->name] = node;
    } else if (node->type == TemplateType::Var) {
        var_templates[current_namespace][node->name] = node;
    }
}

void Generator::visit(const CustomNode* node) {
    if (node->type == CustomType::Style) {
        custom_style_templates[current_namespace][node->name] = node;
    }
}

void Generator::visit(const ElementDirectiveNode* node) {
    const auto& search_namespace = node->from_namespace.empty() ? current_namespace : node->from_namespace;
    auto it = element_templates[search_namespace].find(node->name);
    if (it == element_templates[search_namespace].end() && search_namespace != "") {
        it = element_templates[""].find(node->name);
    }

    if (it != element_templates[search_namespace].end()) {
        for (const auto& child : it->second->children) {
            visit(child.get());
        }
    }
}

void Generator::visit(const ImportNode* node) {
    std::string content = FileUtil::read_file(node->path);
    if (!content.empty()) {
        named_origin_blocks[node->alias] = std::make_unique<OriginNode>(node->type, node->alias, content);
    }
}

std::string Generator::visit(const StyleDirectiveNode* node) {
    std::stringstream style_attr;
    const auto& search_namespace = node->from_namespace.empty() ? current_namespace : node->from_namespace;

    auto it = style_templates[search_namespace].find(node->name);
    if (it == style_templates[search_namespace].end() && search_namespace != "") {
        it = style_templates[""].find(node->name);
    }

    if (it != style_templates[search_namespace].end()) {
        for (const auto& prop : it->second->children) {
            auto prop_node = static_cast<const StylePropertyNode*>(prop.get());
            style_attr << prop_node->key << ":" << prop_node->value << ";";
        }
    }

    auto custom_it = custom_style_templates[search_namespace].find(node->name);
    if (custom_it == custom_style_templates[search_namespace].end() && search_namespace != "") {
        custom_it = custom_style_templates[""].find(node->name);
    }

    if (custom_it != custom_style_templates[search_namespace].end()) {
        for (const auto& prop_name : custom_it->second->valueless_properties) {
            auto prop_it = node->properties.find(prop_name);
            if (prop_it != node->properties.end()) {
                style_attr << prop_name << ":" << prop_it->second << ";";
            }
        }
    }
    return style_attr.str();
}

void Generator::visit(const OriginNode* node) {
    if (!node->name.empty()) {
        // This is a declaration, so we just store it.
        // The generator takes ownership of the node.
        named_origin_blocks[node->name] = std::unique_ptr<OriginNode>(static_cast<OriginNode*>(node->clone().release()));
    } else {
        if (node->type == "Html") {
            html_output << node->content;
        } else if (node->type == "Style") {
            css_output << node->content;
        } else if (node->type == "JavaScript") {
            js_output << node->content;
        }
    }
}

void Generator::visit(const OriginDirectiveNode* node) {
    auto it = named_origin_blocks.find(node->name);
    if (it != named_origin_blocks.end()) {
        if (it->second->type == "Html") {
            html_output << it->second->content;
        } else if (it->second->type == "Style") {
            css_output << it->second->content;
        } else if (it->second->type == "JavaScript") {
            js_output << it->second->content;
        }
    }
}
