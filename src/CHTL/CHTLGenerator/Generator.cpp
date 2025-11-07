#include "Generator.h"
#include "CHTLNode/TemplateUsageNode.h"
#include "CHTLNode/ValueNode.h"
#include "CHTLNode/LiteralValueNode.h"
#include "CHTLNode/VariableUsageNode.h"
#include "CHTLNode/InsertNode.h"
#include <vector>

namespace CHTL {

std::string Generator::generate(const ProgramNode& program) {
    visit(&program);

    std::string css = css_output.str();
    if (!css.empty()) {
        return "<style>" + css + "</style>" + html_output.str();
    }

    return html_output.str();
}

void Generator::visit(const ProgramNode* node) {
    // First pass: collect all style templates
    for (const auto& statement : node->statements) {
        if (statement->getType() == NodeType::Template) {
            visit(static_cast<TemplateNode*>(statement.get()));
        }
    }

    // Second pass: process the rest of the statements
    for (const auto& statement : node->statements) {
        switch (statement->getType()) {
            case NodeType::Element:
                visit(static_cast<ElementNode*>(statement.get()));
                break;
            case NodeType::Origin:
                visit(static_cast<OriginNode*>(statement.get()));
                break;
            case NodeType::TemplateUsage:
                visit(static_cast<TemplateUsageNode*>(statement.get()), nullptr);
                break;
            case NodeType::Template:
                // Already processed
                break;
            default:
                break;
        }
    }
}

void Generator::visit(const ElementNode* node) {
    auto mutableNode = const_cast<ElementNode*>(node);

    // Process style nodes first to populate attributes
    for (const auto& child : node->children) {
        if (child->getType() == NodeType::Style) {
            visit(static_cast<StyleNode*>(child.get()), mutableNode);
        }
    }

    if (!template_usage_context.empty()) {
        auto context = template_usage_context.back();
        int currentIndex = element_indices[node->tagName]++;

        for (const auto& specialization : context->body) {
            if (specialization->getType() == NodeType::Element) {
                auto specElement = static_cast<const ElementNode*>(specialization.get());
                bool tagMatch = specElement->tagName == node->tagName;
                bool indexMatch = specElement->index == -1 || specElement->index == currentIndex;

                if (tagMatch && indexMatch) {
                    for (const auto& child : specElement->children) {
                        if (child->getType() == NodeType::Style) {
                            visit(static_cast<StyleNode*>(child.get()), mutableNode);
                        }
                    }
                }
            }
        }
    }

    html_output << "<" << node->tagName;

    for (const auto& attr : node->attributes) {
        html_output << " " << attr.first << "=\"" << attr.second << "\"";
    }

    html_output << ">";

    for (const auto& child : node->children) {
        switch (child->getType()) {
            case NodeType::Element:
                visit(static_cast<ElementNode*>(child.get()));
                break;
            case NodeType::Text:
                visit(static_cast<TextNode*>(child.get()));
                break;
            case NodeType::Style:
                // Already processed
                break;
            case NodeType::Script:
                visit(static_cast<ScriptNode*>(child.get()));
                break;
            case NodeType::Origin:
                visit(static_cast<OriginNode*>(child.get()));
                break;
            case NodeType::TemplateUsage:
                visit(static_cast<TemplateUsageNode*>(child.get()), const_cast<ElementNode*>(node));
                break;
            default:
                break;
        }
    }

    html_output << "</" << node->tagName << ">";
}

void Generator::visit(const TextNode* node) {
    html_output << node->content;
}

void Generator::visit(const StyleNode* node, ElementNode* parent) {
    std::stringstream styleStream;
    std::string firstClassSelector;
    std::string firstIdSelector;

    // First pass: Find the first class and id selectors to determine the context
    for (const auto& child : node->children) {
        if (child->getType() == NodeType::StyleRule) {
            auto ruleNode = static_cast<const StyleRuleNode*>(child.get());
            const std::string& selector = ruleNode->selector;
            if (!selector.empty()) {
                if (selector[0] == '.' && firstClassSelector.empty()) {
                    firstClassSelector = selector;
                } else if (selector[0] == '#' && firstIdSelector.empty()) {
                    firstIdSelector = selector;
                }
            }
        }
    }

    // Determine the primary selector for '&' replacement (class has priority)
    std::string primarySelectorForContext = !firstClassSelector.empty() ? firstClassSelector : firstIdSelector;

    // Automatically add class or id attributes
    if (!firstClassSelector.empty()) {
        if (parent->attributes.count("class")) {
            parent->attributes["class"] += " " + firstClassSelector.substr(1);
        } else {
            parent->attributes["class"] = firstClassSelector.substr(1);
        }
    }
    if (!firstIdSelector.empty()) {
        parent->attributes["id"] = firstIdSelector.substr(1);
    }

    // Second pass: Generate CSS for all rules
    for (const auto& child : node->children) {
        if (child->getType() == NodeType::StyleProperty) {
            visit(static_cast<StylePropertyNode*>(child.get()), styleStream);
        } else if (child->getType() == NodeType::StyleRule) {
            auto ruleNode = static_cast<const StyleRuleNode*>(child.get());
            std::string resolvedSelector = ruleNode->selector;

            // Resolve '&' selectors
            if (!resolvedSelector.empty() && resolvedSelector[0] == '&') {
                if (!primarySelectorForContext.empty()) {
                    resolvedSelector.replace(0, 1, primarySelectorForContext);
                } else {
                    // If no primary selector, just remove the '&'
                    resolvedSelector.erase(0, 1);
                }
            }

            // Generate the CSS for the rule
            css_output << resolvedSelector << "{";
            for (const auto& prop : ruleNode->properties) {
                visit(prop.get(), css_output);
            }
            css_output << "}";
        }
    }

    // Process style group references
    for (const auto& child : node->children) {
        if (child->getType() == NodeType::TemplateUsage) {
            visit(static_cast<TemplateUsageNode*>(child.get()), parent);
        }
    }

    std::string styleString = styleStream.str();
    if (!styleString.empty()) {
        if (parent->attributes.count("style")) {
            parent->attributes["style"] += styleString;
        } else {
            parent->attributes["style"] = styleString;
        }
    }
}

void Generator::visit(const StylePropertyNode* node, std::stringstream& styleStream) {
    styleStream << node->key << ":";
    for (const auto& valueNode : node->value) {
        if (valueNode->getType() == NodeType::LiteralValue) {
            styleStream << static_cast<LiteralValueNode*>(valueNode.get())->value;
        } else if (valueNode->getType() == NodeType::VariableUsage) {
            auto varUsageNode = static_cast<VariableUsageNode*>(valueNode.get());
            if (var_templates.count(varUsageNode->groupName)) {
                const auto& templateNode = var_templates[varUsageNode->groupName];
                for (const auto& prop : templateNode->body) {
                    auto styleProp = static_cast<StylePropertyNode*>(prop.get());
                    if (styleProp->key == varUsageNode->variableName) {
                        for (const auto& val : styleProp->value) {
                            if (val->getType() == NodeType::LiteralValue) {
                                styleStream << static_cast<LiteralValueNode*>(val.get())->value;
                            }
                        }
                    }
                }
            }
        }
    }
    styleStream << ";";
}

void Generator::visit(const ScriptNode* node) {
    html_output << "<script>" << node->content << "</script>";
}

void Generator::visit(const OriginNode* node) {
    if (node->originType == "Html" || node->originType == "JavaScript") {
        html_output << node->content;
    } else if (node->originType == "Style") {
        css_output << node->content;
    }
}

void Generator::visit(const TemplateNode* node) {
    if (node->type == "Style") {
        style_templates[node->name] = node;
    } else if (node->type == "Element") {
        element_templates[node->name] = node;
    } else if (node->type == "Var") {
        var_templates[node->name] = node;
    }
}

void Generator::visit(const TemplateUsageNode* node, ElementNode* parent) {
    if (node->deleted) {
        return;
    }
    if (node->type == "Style") {
        if (style_templates.count(node->name)) {
            const auto& templateNode = style_templates[node->name];
            std::map<std::string, const StylePropertyNode*> properties;
            std::set<std::string> deletedInheritances;

            if (templateNode->isCustom) {
                for (const auto& item : templateNode->body) {
                    if (item->getType() == NodeType::Delete) {
                        auto deleteNode = static_cast<const DeleteNode*>(item.get());
                        for (const auto& inheritanceName : deleteNode->inheritances) {
                            deletedInheritances.insert(inheritanceName);
                        }
                    }
                }
            }

            resolveStyleInheritance(templateNode, properties, deletedInheritances);

            // Handle provided properties for valueless style groups
            for (const auto& provided_prop : node->provided_properties) {
                if (properties.count(provided_prop->key) && properties[provided_prop->key]->value.empty()) {
                    auto new_prop = std::make_unique<StylePropertyNode>(provided_prop->key, provided_prop->getClonedValues());
                    properties[provided_prop->key] = new_prop.get();
                    owned_properties.push_back(std::move(new_prop));
                }
            }

            std::stringstream styleStream;
            for (const auto& pair : properties) {
                visit(pair.second, styleStream);
            }

            std::string styleString = styleStream.str();
            if (!styleString.empty()) {
                if (parent->attributes.count("style")) {
                    parent->attributes["style"] += styleString;
                } else {
                    parent->attributes["style"] = styleString;
                }
            }
        }
    } else if (node->type == "Element") {
        if (element_templates.count(node->name)) {
            template_usage_context.push_back(node);
            element_indices.clear();
            const auto& templateNode = element_templates[node->name];

            std::vector<const BaseNode*> final_body;
            for(const auto& b : templateNode->body) {
                final_body.push_back(b.get());
            }

            for(const auto& n : node->body) {
                if(n->getType() == NodeType::Insert) {
                    const auto insertNode = static_cast<const InsertNode*>(n.get());

                    if (insertNode->type == InsertType::AtTop) {
                        std::vector<const BaseNode*> insert_body;
                        for (const auto& b : insertNode->body) {
                            insert_body.push_back(b.get());
                        }
                        final_body.insert(final_body.begin(), insert_body.begin(), insert_body.end());
                    } else if (insertNode->type == InsertType::AtBottom) {
                        std::vector<const BaseNode*> insert_body;
                        for (const auto& b : insertNode->body) {
                            insert_body.push_back(b.get());
                        }
                        final_body.insert(final_body.end(), insert_body.begin(), insert_body.end());
                    } else {
                        // For `after`, `before`, `replace`, we need to find the target
                        for (auto it = final_body.begin(); it != final_body.end(); ++it) {
                            if ((*it)->getType() == NodeType::Element) {
                                auto el = static_cast<const ElementNode*>(*it);
                                if (el->tagName == insertNode->target) {
                                    if (insertNode->type == InsertType::After) {
                                        std::vector<const BaseNode*> insert_body;
                                        for (const auto& b : insertNode->body) {
                                            insert_body.push_back(b.get());
                                        }
                                        final_body.insert(it + 1, insert_body.begin(), insert_body.end());
                                        break;
                                    } else if (insertNode->type == InsertType::Before) {
                                        std::vector<const BaseNode*> insert_body;
                                        for (const auto& b : insertNode->body) {
                                            insert_body.push_back(b.get());
                                        }
                                        final_body.insert(it, insert_body.begin(), insert_body.end());
                                        break;
                                    } else if (insertNode->type == InsertType::Replace) {
                                        it = final_body.erase(it);
                                        std::vector<const BaseNode*> insert_body;
                                        for (const auto& b : insertNode->body) {
                                            insert_body.push_back(b.get());
                                        }
                                        final_body.insert(it, insert_body.begin(), insert_body.end());
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }


            for (const auto& statement : final_body) {
                switch (statement->getType()) {
                    case NodeType::Element:
                        visit(static_cast<const ElementNode*>(statement));
                        break;
                    case NodeType::Text:
                        visit(static_cast<const TextNode*>(statement));
                        break;
                    case NodeType::Style:
                        visit(static_cast<const StyleNode*>(statement), parent);
                        break;
                    case NodeType::Script:
                        visit(static_cast<const ScriptNode*>(statement));
                        break;
                    case NodeType::Origin:
                        visit(static_cast<const OriginNode*>(statement));
                        break;
                    case NodeType::TemplateUsage:
                        visit(static_cast<const TemplateUsageNode*>(statement), parent);
                        break;
                    default:
                        break;
                }
            }
            template_usage_context.pop_back();
        }
    }
}

void Generator::resolveStyleInheritance(const TemplateNode* node, std::map<std::string, const StylePropertyNode*>& properties, const std::set<std::string>& deletedInheritances) {
    // Check for circular dependencies
    for (const auto& name : inheritance_stack) {
        if (name == node->name) {
            throw std::runtime_error("Circular dependency detected in style template inheritance: " + node->name);
        }
    }

    inheritance_stack.push_back(node->name);

    // First, resolve parent templates
    for (const auto& inheritance : node->inheritances) {
        if (deletedInheritances.find(inheritance->name) == deletedInheritances.end() && style_templates.count(inheritance->name)) {
            resolveStyleInheritance(style_templates[inheritance->name], properties, deletedInheritances);
        }
    }

    // Then, process this template's body
    for (const auto& item : node->body) {
        if (item->getType() == NodeType::StyleProperty) {
            auto styleProp = static_cast<const StylePropertyNode*>(item.get());
            properties[styleProp->key] = styleProp;
        } else if (item->getType() == NodeType::Delete && node->isCustom) {
            auto deleteNode = static_cast<const DeleteNode*>(item.get());
            for (const auto& propName : deleteNode->properties) {
                properties.erase(propName);
            }
        }
    }

    inheritance_stack.pop_back();
}

} // namespace CHTL
