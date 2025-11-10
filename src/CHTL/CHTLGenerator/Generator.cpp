#include "Generator.h"
#include <algorithm>
#include "../CHTLLexer/Lexer.h"
#include "../CHTLParser/Parser.h"
#include "CHTLNode/TemplateUsageNode.h"
#include "CHTLNode/ValueNode.h"
#include "CHTLNode/LiteralValueNode.h"
#include "CHTLNode/VariableUsageNode.h"
#include "CHTLNode/InsertNode.h"
#include "CHTLNode/ElementDeleteNode.h"
#include "CHTLNode/BinaryOperationNode.h"
#include "CHTLNode/PropertyReferenceNode.h"
#include "CHTLNode/ConditionalNode.h"
#include "CHTLNode/ComparisonNode.h"
#include "CHTLNode/IfNode.h"
#include "CHTLNode/ElseNode.h"
#include "CHTLNode/LogicalNode.h"
#include "CHTLNode/ImportNode.h"
#include "CHTLNode/NamespaceNode.h"
#include "CHTLNode/ConfigNode.h"
#include "CHTLNode/UseNode.h"
#include <vector>
#include <fstream>
#include <cstring>

namespace CHTL {

Generator::Generator(const Configuration& config) : config(config) {}

void Generator::collect_symbols(const BaseNode* node) {
    if (node->getType() == NodeType::Element) {
        auto elementNode = static_cast<const ElementNode*>(node);
        std::string selector;
        if (elementNode->attributes.count("id")) {
            selector = "#" + elementNode->attributes.at("id");
        } else if (elementNode->attributes.count("class")) {
            selector = "." + elementNode->attributes.at("class");
        } else {
            selector = elementNode->tagName;
        }

        for (const auto& child : elementNode->children) {
            if (child->getType() == NodeType::Style) {
                auto styleNode = static_cast<const StyleNode*>(child.get());
                for (const auto& styleChild : styleNode->children) {
                    if (styleChild->getType() == NodeType::StyleProperty) {
                        auto prop = static_cast<const StylePropertyNode*>(styleChild.get());
                        if (!prop->value.empty()) {
                            symbol_table[selector][prop->key] = prop->value[0].get();
                        }
                    }
                }
            }
        }

        for (const auto& child : elementNode->children) {
            collect_symbols(child.get());
        }
    } else if (node->getType() == NodeType::Program) {
        auto programNode = static_cast<const ProgramNode*>(node);
        for (const auto& statement : programNode->statements) {
            collect_symbols(statement.get());
        }
    }
}

void Generator::visit(const ImportNode* node) {
    std::ifstream file(node->path);
    if (!file) {
        throw std::runtime_error("Failed to open import file: " + node->path + " - " + strerror(errno));
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    content.erase(content.find_last_not_of(" \n\r\t")+1);

    if (!node->qualifier.empty() || node->type == "Chtl") {
        Lexer lexer(content);
        Parser parser(lexer);
        auto program = parser.parse();
        for (auto& statement : program->statements) {
            bool should_import = node->itemName.empty();
            if (!should_import) {
                if (statement->getType() == NodeType::Template) {
                    auto templateNode = static_cast<const TemplateNode*>(statement.get());
                    if (templateNode->name == node->itemName) {
                        should_import = true;
                    }
                } else if (statement->getType() == NodeType::Origin) {
                    auto originNode = static_cast<const OriginNode*>(statement.get());
                    if (originNode->name == node->itemName) {
                        should_import = true;
                    }
                }
            }

            if (should_import) {
                const BaseNode* raw_stmt = statement.get();
                owned_nodes.push_back(std::move(statement));

                if (raw_stmt->getType() == NodeType::Template) {
                    visit(static_cast<const TemplateNode*>(raw_stmt));
                } else if (raw_stmt->getType() == NodeType::Origin) {
                    const auto originNode = static_cast<const OriginNode*>(raw_stmt);
                    if (!originNode->name.empty()) {
                        named_origins[originNode->name] = originNode;
                    }
                }
            }
        }
        program->statements.clear();
    } else {
        auto originNode = std::make_unique<OriginNode>(node->type, content, node->alias);
        named_origins[node->alias] = originNode.get();
        owned_nodes.push_back(std::move(originNode));
    }
}

void Generator::visit(const BaseNode* node) {
    switch (node->getType()) {
        case NodeType::Element:
            visit(static_cast<const ElementNode*>(node));
            break;
        case NodeType::Text:
            visit(static_cast<const TextNode*>(node));
            break;
        case NodeType::If:
            visit(static_cast<const IfNode*>(node));
            break;
        case NodeType::Else:
            visit(static_cast<const ElseNode*>(node));
            break;
        case NodeType::Import:
            visit(static_cast<const ImportNode*>(node));
            break;
        case NodeType::Namespace:
            visit(static_cast<const NamespaceNode*>(node));
            break;
        case NodeType::Config:
            visit(static_cast<const ConfigNode*>(node));
            break;
        case NodeType::Use:
			visit(static_cast<const UseNode*>(node));
			break;
        default:
            break;
    }
}

bool Generator::evaluateCondition(const ValueNode* condition) {
    if (condition->getType() == NodeType::Comparison) {
        auto comparisonNode = static_cast<const ComparisonNode*>(condition);
        if (comparisonNode->left->getType() == NodeType::LiteralValue && comparisonNode->right->getType() == NodeType::LiteralValue) {
            try {
                double left = std::stod(static_cast<const LiteralValueNode*>(comparisonNode->left.get())->value);
                double right = std::stod(static_cast<const LiteralValueNode*>(comparisonNode->right.get())->value);
                if (comparisonNode->op == ">") return left > right;
                if (comparisonNode->op == "<") return left < right;
                if (comparisonNode->op == ">=") return left >= right;
                if (comparisonNode->op == "<=") return left <= right;
                if (comparisonNode->op == "==") return left == right;
                if (comparisonNode->op == "!=") return left != right;
            } catch (const std::invalid_argument& e) {
                // Handle non-numeric values
                std::string left = static_cast<const LiteralValueNode*>(comparisonNode->left.get())->value;
                std::string right = static_cast<const LiteralValueNode*>(comparisonNode->right.get())->value;
                if (comparisonNode->op == "==") return left == right;
                if (comparisonNode->op == "!=") return left != right;
            }
        }
    } else if (condition->getType() == NodeType::Logical) {
        auto logicalNode = static_cast<const LogicalNode*>(condition);
        bool left = evaluateCondition(logicalNode->left.get());
        bool right = evaluateCondition(logicalNode->right.get());
        if (logicalNode->op == "&&") return left && right;
        if (logicalNode->op == "||") return left || right;
    }
    return false;
}

void Generator::visit(const ConditionalNode* node, std::stringstream& styleStream) {
    if (evaluateCondition(node->condition.get())) {
        if (node->true_branch->getType() == NodeType::LiteralValue) {
            styleStream << static_cast<const LiteralValueNode*>(node->true_branch.get())->value;
        }
    } else {
        if (node->false_branch->getType() == NodeType::LiteralValue) {
            styleStream << static_cast<const LiteralValueNode*>(node->false_branch.get())->value;
        }
    }
}

std::string Generator::generate(const ProgramNode& program) {
    visit(&program);

    std::string css = css_output.str();
    if (!css.empty()) {
        return "<style>" + css + "</style>" + html_output.str();
    }

    return html_output.str();
}

void Generator::visit(const ProgramNode* node) {
    // First pass: collect all templates, imports, and named origins.
    for (const auto& statement : node->statements) {
        if (statement->getType() == NodeType::Template) {
            visit(static_cast<const TemplateNode*>(statement.get()));
        } else if (statement->getType() == NodeType::Import) {
            visit(static_cast<const ImportNode*>(statement.get()));
        } else if (statement->getType() == NodeType::Origin) {
            const auto originNode = static_cast<const OriginNode*>(statement.get());
            if (!originNode->name.empty()) {
                named_origins[originNode->name] = originNode;
            }
        } else if (statement->getType() == NodeType::Namespace) {
            visit(static_cast<const NamespaceNode*>(statement.get()));
        } else if (statement->getType() == NodeType::Use) {
            visit(static_cast<const UseNode*>(statement.get()));
        } else if (statement->getType() == NodeType::Config) {
            const auto configNode = static_cast<const ConfigNode*>(statement.get());
            if (!configNode->name.empty()) {
                named_configs[configNode->name] = configNode;
            } else {
                visit(configNode);
            }
        }
    }

    // Second pass: collect symbols and populate attributes
    for (const auto& statement : node->statements) {
        if (statement->getType() == NodeType::Element) {
            collect_symbols(statement.get());
        }
    }

    // Third pass: generate output for everything else.
    for (const auto& statement : node->statements) {
        switch (statement->getType()) {
        case NodeType::Element:
            visit(static_cast<const ElementNode*>(statement.get()));
            break;
        case NodeType::Origin:
            // Only generate for UNNAMED origins. Named ones are generated on usage.
        {
            const auto originNode = static_cast<const OriginNode*>(statement.get());
            if (originNode->name.empty()) {
                visit(originNode);
            }
        }
        break;
        case NodeType::TemplateUsage:
            visit(static_cast<const TemplateUsageNode*>(statement.get()), nullptr);
            break;
        case NodeType::Template:
            // Already processed
            break;
        case NodeType::If:
            visit(statement.get());
            break;
        case NodeType::Import:
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

    if (config.debugMode) {
        html_output << "<!-- Start Element: " << node->tagName << " -->";
    }
    html_output << "<" << node->tagName;

    for (const auto& attr : node->attributes) {
        html_output << " " << attr.first << "=\"" << attr.second << "\"";
    }

    html_output << ">";

    for (const auto& child : node->children) {
        for (const auto& constraint : node->constraints) {
            if (child->getType() == NodeType::Element) {
                auto childElement = static_cast<const ElementNode*>(child.get());
                if (childElement->tagName == constraint) {
                    throw std::runtime_error("Constraint violation: element " + childElement->tagName + " is not allowed.");
                }
            } else if (child->getType() == NodeType::TemplateUsage) {
                auto childTemplate = static_cast<const TemplateUsageNode*>(child.get());
                if (childTemplate->name == constraint) {
                    throw std::runtime_error("Constraint violation: template " + childTemplate->name + " is not allowed.");
                }
            }
        }
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
    if (config.debugMode) {
        html_output << "<!-- End Element: " << node->tagName << " -->";
    }
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
    if (!config.disableStyleAutoAddClass && !firstClassSelector.empty()) {
        if (parent->attributes.count("class")) {
            parent->attributes["class"] += " " + firstClassSelector.substr(1);
        } else {
            parent->attributes["class"] = firstClassSelector.substr(1);
        }
    }
    if (!config.disableStyleAutoAddId && !firstIdSelector.empty()) {
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
            std::string fullName = varUsageNode->from.empty() ? varUsageNode->groupName : varUsageNode->from + "." + varUsageNode->groupName;
            if (var_templates.count(fullName)) {
                const auto& templateNode = var_templates[fullName];
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
        } else if (valueNode->getType() == NodeType::BinaryOperation) {
            visit(static_cast<BinaryOperationNode*>(valueNode.get()), styleStream);
        } else if (valueNode->getType() == NodeType::PropertyReference) {
            visit(static_cast<PropertyReferenceNode*>(valueNode.get()), styleStream);
        } else if (valueNode->getType() == NodeType::Conditional) {
            visit(static_cast<ConditionalNode*>(valueNode.get()), styleStream);
        }
    }
    styleStream << ";";
}

void Generator::visit(const BinaryOperationNode* node, std::stringstream& styleStream) {
    styleStream << "calc(";
    // Left operand
    if (node->left->getType() == NodeType::LiteralValue) {
        styleStream << static_cast<LiteralValueNode*>(node->left.get())->value;
    } else if (node->left->getType() == NodeType::BinaryOperation) {
        visit(static_cast<BinaryOperationNode*>(node->left.get()), styleStream);
    }

    styleStream << " " << node->op << " ";

    // Right operand
    if (node->right->getType() == NodeType::LiteralValue) {
        styleStream << static_cast<LiteralValueNode*>(node->right.get())->value;
    } else if (node->right->getType() == NodeType::BinaryOperation) {
        visit(static_cast<BinaryOperationNode*>(node->right.get()), styleStream);
    }
    styleStream << ")";
}

void Generator::visit(const PropertyReferenceNode* node, std::stringstream& styleStream) {
    if (symbol_table.count(node->selector) && symbol_table[node->selector].count(node->propertyName)) {
        auto valueNode = symbol_table[node->selector][node->propertyName];
        if (valueNode->getType() == NodeType::LiteralValue) {
            styleStream << static_cast<const LiteralValueNode*>(valueNode)->value;
        }
    }
}

void Generator::visit(const ScriptNode* node) {
    html_output << "<script>" << node->content << "</script>";
}

void Generator::visit(const OriginNode* node) {
    if (node->originType == "Html" || node->originType == "JavaScript") {
        html_output << node->content;
    }
    else if (node->originType == "Style") {
        css_output << node->content;
    }
}

void Generator::visit(const TemplateNode* node) {
    std::string fullName = node->name;
    if (!namespace_stack.empty()) {
        std::string prefix;
        for (const auto& ns : namespace_stack) {
            prefix += ns + ".";
        }
        fullName = prefix + node->name;
    }

    if (node->type == "Style") {
        style_templates[fullName] = node;
    } else if (node->type == "Element") {
        element_templates[fullName] = node;
    } else if (node->type == "Var") {
        var_templates[fullName] = node;
    }
}

void Generator::visit(const TemplateUsageNode* node, ElementNode* parent) {
    if (node->deleted) {
        return;
    }
    if (named_origins.count(node->name)) {
        // It's a named origin usage, like @Html myFile;
        const auto originNode = named_origins[node->name];
        if (originNode->originType == "Html" || originNode->originType == "JavaScript") {
            html_output << originNode->content;
        }
        else if (originNode->originType == "Style") {
            css_output << originNode->content;
        }
        return;
    }

    std::string fullName = node->from.empty() ? node->name : node->from + "." + node->name;

    if (node->type == "Style") {
        if (style_templates.count(fullName)) {
            const auto& templateNode = style_templates[fullName];
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
        if (element_templates.count(fullName)) {
            template_usage_context.push_back(node);
            element_indices.clear();
            const auto& templateNode = element_templates[fullName];

            std::vector<const BaseNode*> final_body;
            resolveElementInheritance(templateNode, final_body);

            for(const auto& n : node->body) {
                if(n->getType() == NodeType::ElementDelete) {
                    const auto deleteNode = static_cast<const ElementDeleteNode*>(n.get());
                    for(const auto& target : deleteNode->targets) {
                        if(target->getType() == NodeType::Element) {
                            const auto targetElement = static_cast<const ElementNode*>(target.get());
                            final_body.erase(std::remove_if(final_body.begin(), final_body.end(), [&](const BaseNode* b){
                                if(b->getType() == NodeType::Element) {
                                    const auto el = static_cast<const ElementNode*>(b);
                                    return el->tagName == targetElement->tagName && (targetElement->index == -1 || el->index == targetElement->index);
                                }
                                return false;
                            }), final_body.end());
                        } else if (target->getType() == NodeType::TemplateUsage) {
                            const auto targetTemplate = static_cast<const TemplateUsageNode*>(target.get());
                            final_body.erase(std::remove_if(final_body.begin(), final_body.end(), [&](const BaseNode* b){
                                if(b->getType() == NodeType::TemplateUsage) {
                                    const auto tu = static_cast<const TemplateUsageNode*>(b);
                                    return tu->name == targetTemplate->name;
                                }
                                return false;
                            }), final_body.end());
                        }
                    }
                } else if(n->getType() == NodeType::Insert) {
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

void Generator::resolveElementInheritance(const TemplateNode* node, std::vector<const BaseNode*>& body) {
    // Check for circular dependencies
    for (const auto& name : inheritance_stack) {
        if (name == node->name) {
            throw std::runtime_error("Circular dependency detected in element template inheritance: " + node->name);
        }
    }

    inheritance_stack.push_back(node->name);

    // First, resolve parent templates
    for (const auto& inheritance : node->inheritances) {
        if (element_templates.count(inheritance->name)) {
            resolveElementInheritance(element_templates[inheritance->name], body);
        }
    }

    // Then, process this template's body
    for (const auto& item : node->body) {
        if (item->getType() == NodeType::ElementDelete) {
            const auto deleteNode = static_cast<const ElementDeleteNode*>(item.get());
            for(const auto& target : deleteNode->targets) {
                if(target->getType() == NodeType::Element) {
                    const auto targetElement = static_cast<const ElementNode*>(target.get());
                    if (targetElement->index != -1) {
                        // Indexed deletion
                        int count = 0;
                        for (auto it = body.begin(); it != body.end(); ) {
                            if ((*it)->getType() == NodeType::Element) {
                                const auto el = static_cast<const ElementNode*>(*it);
                                if (el->tagName == targetElement->tagName) {
                                    if (count == targetElement->index) {
                                        it = body.erase(it);
                                        break;
                                    }
                                    count++;
                                }
                            }
                            ++it;
                        }
                    } else {
                        // Unindexed deletion
                        body.erase(std::remove_if(body.begin(), body.end(), [&](const BaseNode* b){
                            if(b->getType() == NodeType::Element) {
                                const auto el = static_cast<const ElementNode*>(b);
                                return el->tagName == targetElement->tagName;
                            }
                            return false;
                        }), body.end());
                    }
                } else if (target->getType() == NodeType::TemplateUsage) {
                    const auto targetTemplate = static_cast<const TemplateUsageNode*>(target.get());
                    body.erase(std::remove_if(body.begin(), body.end(), [&](const BaseNode* b){
                        if(b->getType() == NodeType::TemplateUsage) {
                            const auto tu = static_cast<const TemplateUsageNode*>(b);
                            return tu->name == targetTemplate->name;
                        }
                        return false;
                    }), body.end());
                }
            }
        } else {
            body.push_back(item.get());
        }
    }

    inheritance_stack.pop_back();
}

void Generator::visit(const IfNode* node) {
    if (evaluateCondition(node->condition.get())) {
        for (const auto& statement : node->body) {
            if (statement) {
                visit(statement.get());
            }
        }
    } else {
        for (const auto& else_branch : node->else_branches) {
            visit(else_branch.get());
        }
    }
}

void Generator::visit(const ElseNode* node) {
    if (node->condition) {
        if (evaluateCondition(node->condition.get())) {
            for (const auto& statement : node->body) {
                if (statement) {
                    visit(statement.get());
                }
            }
        }
    } else {
        for (const auto& statement : node->body) {
            if (statement) {
                visit(statement.get());
            }
        }
    }
}

void Generator::visit(const NamespaceNode* node) {
    namespace_stack.push_back(node->name);
    for (const auto& statement : node->body) {
        if (statement->getType() == NodeType::Template) {
            visit(static_cast<const TemplateNode*>(statement.get()));
        } else if (statement->getType() == NodeType::Namespace) {
            visit(static_cast<const NamespaceNode*>(statement.get()));
        }
    }
    namespace_stack.pop_back();
}

void Generator::visit(const ConfigNode* node) {
    // Config is applied at generation time, this is for completeness.
}

void Generator::visit(const UseNode* node) {
    if (node->type == "html5") {
        html_output << "<!DOCTYPE html>";
    } else if (node->type == "@Config") {
        if (named_configs.count(node->configName)) {
            const auto configNode = named_configs[node->configName];
            for (const auto& setting : configNode->settings) {
                if (setting.first == "DEBUG_MODE") {
                    config.debugMode = (setting.second == "true");
                } else if (setting.first == "DISABLE_STYLE_AUTO_ADD_CLASS") {
                    config.disableStyleAutoAddClass = (setting.second == "true");
                } else if (setting.first == "DISABLE_STYLE_AUTO_ADD_ID") {
                    config.disableStyleAutoAddId = (setting.second == "true");
                }
            }
        }
    }
}

} // namespace CHTL
