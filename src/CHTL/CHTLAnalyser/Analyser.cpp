#include "Analyser.h"
#include <stdexcept>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <filesystem>


namespace CHTL {

Analyser::Analyser(ProgramNode& program, std::string filePath) : m_program(program), m_filePath(filePath) {}

void Analyser::analyse() {
    visit(&m_program);

    for (auto const& [name, node] : m_templates) {
        resolveInheritance(node);
    }

    resolve(&m_program);
}

void Analyser::visit(ASTNode* node) {
    if (!node) return;

    switch (node->getType()) {
        case NodeType::Program:
            visit(static_cast<ProgramNode*>(node));
            break;
        case NodeType::Element:
            visit(static_cast<ElementNode*>(node));
            break;
        case NodeType::Template:
            visit(static_cast<TemplateNode*>(node));
            break;
        case NodeType::Import:
            visit(static_cast<ImportNode*>(node));
            break;
        case NodeType::Namespace:
            visit(static_cast<NamespaceNode*>(node));
            break;
        default:
            break;
    }
}

void Analyser::visit(ProgramNode* node) {
    size_t i = 0;
    while (i < node->children.size()) {
        auto& child = node->children[i];
        if (child->getType() == NodeType::Namespace) {
            auto ns_node = std::move(child);
            node->children.erase(node->children.begin() + i);
            auto* ns = static_cast<NamespaceNode*>(ns_node.get());
            node->children.insert(node->children.begin() + i,
                                  std::make_move_iterator(ns->children.begin()),
                                  std::make_move_iterator(ns->children.end()));
        } else {
            visit(child.get());
            i++;
        }
    }
}

void Analyser::visit(ElementNode* node) {
    std::vector<std::string> selectors;
    selectors.push_back(node->tagName);
    std::string id_selector;
    for (const auto& attr : node->attributes) {
        if (attr.key == "id") {
            id_selector = "#" + attr.value;
            break;
        }
    }
    if (id_selector.empty() && !node->auto_ids.empty()) {
        id_selector = "#" + node->auto_ids[0];
    }
    if (!id_selector.empty()) {
        selectors.push_back(id_selector);
    }
    for (const auto& attr : node->attributes) {
        if (attr.key == "class") {
            std::istringstream iss(attr.value);
            std::string className;
            while (iss >> className) {
                selectors.push_back("." + className);
            }
        }
    }
    for (const auto& className : node->auto_classes) {
        selectors.push_back("." + className);
    }

    if (node->style) {
        for (const auto& selector : selectors) {
            if (m_symbol_table.find(selector) == nullptr) {
                m_symbol_table.insert(selector, node);
                for (const auto& prop : node->style->properties) {
                    m_symbol_table.addProperty(selector, prop.key, prop.value.get());
                }
            }
        }
    }

    size_t i = 0;
    while (i < node->children.size()) {
        auto& child = node->children[i];
        if (child->getType() == NodeType::Namespace) {
            auto ns_node = std::move(child);
            node->children.erase(node->children.begin() + i);
            auto* ns = static_cast<NamespaceNode*>(ns_node.get());
            node->children.insert(node->children.begin() + i,
                                  std::make_move_iterator(ns->children.begin()),
                                  std::make_move_iterator(ns->children.end()));
        } else {
            visit(child.get());
            i++;
        }
    }
}

void Analyser::visit(TemplateNode* node) {
    if (m_templates.count(node->name)) {
        throw std::runtime_error("Template with name '" + node->name + "' already defined.");
    }
    m_templates[node->name] = node;
}

void Analyser::visit(NamespaceNode* node) {
    // This is now handled by the flattening logic in visit(ProgramNode*) and visit(ElementNode*).
    // This function should not be called.
}

void Analyser::visit(ImportNode* node) {
    // This is now handled in visit(ProgramNode*)
}

void Analyser::resolveInheritance(TemplateNode* node) {
    if (m_inheritance_stack.count(node->name)) {
        throw std::runtime_error("Circular template inheritance detected: " + node->name);
    }
    m_inheritance_stack.insert(node->name);

    std::vector<std::unique_ptr<ASTNode>> inherited_children;
    std::vector<StyleProperty> inherited_properties;

    for (const auto& inheritance : node->inheritances) {
        auto it = m_templates.find(inheritance->name);
        if (it == m_templates.end()) {
            throw std::runtime_error("Unknown template in inheritance: " + inheritance->name);
        }
        TemplateNode* parent = it->second;
        resolveInheritance(parent); // Recurse

        if (node->templateType == TemplateType::Element) {
            for (const auto& child : parent->children) {
                inherited_children.push_back(child->clone());
            }
        } else { // Style
            for (const auto& prop : parent->properties) {
                inherited_properties.push_back({prop.key, prop.value ? prop.value->clone() : nullptr});
            }
        }
    }

    // Prepend inherited children
    node->children.insert(node->children.begin(), std::make_move_iterator(inherited_children.begin()), std::make_move_iterator(inherited_children.end()));

	// Filter out deleted properties from the inherited list
	if (!node->deleted_properties.empty()) {
		inherited_properties.erase(std::remove_if(inherited_properties.begin(), inherited_properties.end(),
			[&](const StyleProperty& prop) {
				return std::find(node->deleted_properties.begin(), node->deleted_properties.end(), prop.key) != node->deleted_properties.end();
			}), inherited_properties.end());
	}

    // Combine properties, giving precedence to the current node's properties.
    std::vector<StyleProperty> final_properties;
    std::unordered_set<std::string> defined_keys;

    // First, add all properties from the current node and track their keys.
    for (auto& prop : node->properties) {
        defined_keys.insert(prop.key);
        final_properties.push_back(std::move(prop));
    }
    node->properties.clear();

    // Then, add properties from the parent, but only if they haven't been defined in the current node.
    for (auto it = inherited_properties.rbegin(); it != inherited_properties.rend(); ++it) {
        if (defined_keys.find(it->key) == defined_keys.end()) {
            final_properties.insert(final_properties.begin(), std::move(*it));
        }
    }

    node->properties = std::move(final_properties);

    m_inheritance_stack.erase(node->name);
}

void Analyser::resolve(ASTNode* node) {
    if (!node) return;

    switch (node->getType()) {
        case NodeType::Program:
            resolve(static_cast<ProgramNode*>(node));
            break;
        case NodeType::Element:
            resolve(static_cast<ElementNode*>(node));
            break;
        case NodeType::Style:
            resolve(static_cast<StyleNode*>(node));
            break;
        case NodeType::BinaryOp:
            resolve(static_cast<BinaryOpNode*>(node)->left);
            resolve(static_cast<BinaryOpNode*>(node)->right);
            break;
        case NodeType::TemplateUsage:
            resolve(static_cast<TemplateUsageNode*>(node));
            break;
        default:
            break;
    }
}

void Analyser::resolve(ProgramNode* node) {
    size_t i = 0;
    while (i < node->children.size()) {
        auto& child = node->children[i];
        bool node_was_replaced = false;

        if (child->getType() == NodeType::Import) {
            auto* import_node = static_cast<ImportNode*>(child.get());
            std::string canonical_path = std::filesystem::weakly_canonical(std::filesystem::path(m_filePath).parent_path() / import_node->filePath).string();

            if (m_import_stack.count(canonical_path)) {
                throw std::runtime_error("Circular import detected: " + canonical_path);
            }
            m_import_stack.insert(canonical_path);

            if (import_node->type == ImportType::Chtl) {
                auto imported_ast = m_importer.importFile(import_node->filePath, m_filePath);
                Analyser imported_analyser(*static_cast<ProgramNode*>(imported_ast.get()), canonical_path);
                imported_analyser.m_import_stack = m_import_stack;
                imported_analyser.analyse();

                auto* imported_program = static_cast<ProgramNode*>(imported_ast.get());

                node->children.erase(node->children.begin() + i);
                node->children.insert(node->children.begin() + i,
                                      std::make_move_iterator(imported_program->children.begin()),
                                      std::make_move_iterator(imported_program->children.end()));
                i += imported_program->children.size();
                node_was_replaced = true;
            } else {
                std::string content = m_importer.importRawFile(import_node->filePath, m_filePath);
                auto origin_node = std::make_unique<OriginNode>(content, import_node->type);
                node->children[i] = std::move(origin_node);
            }

            m_import_stack.erase(canonical_path);
        }
        else if (child->getType() == NodeType::TemplateUsage) {
            auto* usage_node = static_cast<TemplateUsageNode*>(child.get());
            if (usage_node->templateType == TemplateType::Element) {
                auto it = m_templates.find(usage_node->name);
                if (it == m_templates.end()) {
                    throw std::runtime_error("Unknown template: " + usage_node->name);
                }
                const TemplateNode* templateNode = it->second;
                std::vector<std::unique_ptr<ASTNode>> expanded_children;
                for (const auto& template_child : templateNode->children) {
                    expanded_children.push_back(template_child->clone());
                }
                applySpecializations(expanded_children, usage_node);

                node->children.erase(node->children.begin() + i);
                node->children.insert(node->children.begin() + i,
                                      std::make_move_iterator(expanded_children.begin()),
                                      std::make_move_iterator(expanded_children.end()));
                i += expanded_children.size();
                node_was_replaced = true;
            }
        }
        else {
            resolve(child.get());
        }

        if (!node_was_replaced) {
            i++;
        }
    }
}

void Analyser::resolve(ElementNode* node) {
    if (node->style) {
        resolve(node->style.get());
    }

    size_t i = 0;
    while (i < node->children.size()) {
        auto& child = node->children[i];
        bool node_was_replaced = false;

        if (child->getType() == NodeType::Import) {
            auto* import_node = static_cast<ImportNode*>(child.get());
            std::string canonical_path = std::filesystem::weakly_canonical(std::filesystem::path(m_filePath).parent_path() / import_node->filePath).string();

            if (m_import_stack.count(canonical_path)) {
                throw std::runtime_error("Circular import detected: " + canonical_path);
            }
            m_import_stack.insert(canonical_path);

            if (import_node->type == ImportType::Chtl) {
                auto imported_ast = m_importer.importFile(import_node->filePath, m_filePath);
                Analyser imported_analyser(*static_cast<ProgramNode*>(imported_ast.get()), canonical_path);
                imported_analyser.m_import_stack = m_import_stack;
                imported_analyser.analyse();

                auto* imported_program = static_cast<ProgramNode*>(imported_ast.get());

                node->children.erase(node->children.begin() + i);
                node->children.insert(node->children.begin() + i,
                                      std::make_move_iterator(imported_program->children.begin()),
                                      std::make_move_iterator(imported_program->children.end()));
                i += imported_program->children.size();
                node_was_replaced = true;
            } else {
                std::string content = m_importer.importRawFile(import_node->filePath, m_filePath);
                auto origin_node = std::make_unique<OriginNode>(content, import_node->type);
                node->children[i] = std::move(origin_node);
            }

            m_import_stack.erase(canonical_path);
        }
        else if (child->getType() == NodeType::TemplateUsage) {
            auto* usage_node = static_cast<TemplateUsageNode*>(child.get());
            if (usage_node->templateType == TemplateType::Element) {
                auto it = m_templates.find(usage_node->name);
                if (it == m_templates.end()) {
                    throw std::runtime_error("Unknown template: " + usage_node->name);
                }
                const TemplateNode* templateNode = it->second;
                std::vector<std::unique_ptr<ASTNode>> expanded_children;
                for (const auto& template_child : templateNode->children) {
                    expanded_children.push_back(template_child->clone());
                }
                applySpecializations(expanded_children, usage_node);

                node->children.erase(node->children.begin() + i);
                node->children.insert(node->children.begin() + i,
                                      std::make_move_iterator(expanded_children.begin()),
                                      std::make_move_iterator(expanded_children.end()));
                i += expanded_children.size();
                node_was_replaced = true;
            }
        }
        else {
            resolve(child.get());
        }

        if (!node_was_replaced) {
            i++;
        }
    }
}

void Analyser::resolve(StyleNode* node) {
    std::vector<StyleProperty> new_properties;
    for (const auto& usage : node->template_usages) {
        if (usage->templateType == TemplateType::Style) {
            auto it = m_templates.find(usage->name);
            if (it == m_templates.end()) {
                throw std::runtime_error("Unknown style template: " + usage->name);
            }
            TemplateNode* templateNode = it->second;

            for (const auto& prop : templateNode->properties) {
                if (prop.value) { // Not a placeholder
                    new_properties.push_back({prop.key, prop.value->clone()});
                } else { // Placeholder
                    auto val_it = std::find_if(usage->provided_values.begin(), usage->provided_values.end(),
                                               [&](const StyleProperty& p) { return p.key == prop.key; });
                    if (val_it == usage->provided_values.end()) {
                        throw std::runtime_error("Missing value for placeholder '" + prop.key + "' in custom style '" + usage->name + "'");
                    }
                    new_properties.push_back({prop.key, val_it->value->clone()});
                }
            }
        }
		// Handle deletions after processing all properties for this usage
		if (!usage->deleted_properties.empty()) {
			new_properties.erase(std::remove_if(new_properties.begin(), new_properties.end(),
				[&](const StyleProperty& prop) {
					return std::find(usage->deleted_properties.begin(), usage->deleted_properties.end(), prop.key) != usage->deleted_properties.end();
				}), new_properties.end());
		}
    }
     node->properties.insert(node->properties.begin(), std::make_move_iterator(new_properties.begin()), std::make_move_iterator(new_properties.end()));


    for (auto& prop : node->properties) {
        resolve(prop);
    }
    for (auto& block : node->selector_blocks) {
        for (auto& prop : block->properties) {
            resolve(prop);
        }
    }
}

void Analyser::resolve(std::unique_ptr<ASTNode>& node) {
    if (!node) return;

    switch (node->getType()) {
        case NodeType::BinaryOp: {
            auto* bin_op_node = static_cast<BinaryOpNode*>(node.get());
            resolve(bin_op_node->left);
            resolve(bin_op_node->right);
            break;
        }
        case NodeType::TernaryOp: {
            auto* ternary_node = static_cast<TernaryOpNode*>(node.get());
            resolve(ternary_node->condition);
            resolve(ternary_node->then_expr);
            resolve(ternary_node->else_expr);
            break;
        }
        case NodeType::PropertyAccess: {
            auto* access_node = static_cast<PropertyAccessNode*>(node.get());
            const Symbol* symbol = m_symbol_table.find(access_node->selector);
            if (!symbol) {
                throw std::runtime_error("Unknown selector in property access: " + access_node->selector);
            }
            auto it = symbol->properties.find(access_node->property);
            if (it == symbol->properties.end()) {
                throw std::runtime_error("Unknown property in property access: " + access_node->property);
            }
            node = it->second->clone();
            break;
        }
        case NodeType::VarAccess: {
            auto* var_node = static_cast<VarAccessNode*>(node.get());
            auto it = m_templates.find(var_node->templateName);
            if (it == m_templates.end() || it->second->templateType != TemplateType::Var) {
                throw std::runtime_error("Unknown var template: " + var_node->templateName);
            }
            const TemplateNode* templateNode = it->second;
            auto prop_it = std::find_if(templateNode->properties.begin(), templateNode->properties.end(),
                                        [&](const StyleProperty& prop) { return prop.key == var_node->property; });
            if (prop_it == templateNode->properties.end()) {
                throw std::runtime_error("Unknown property '" + var_node->property + "' in var template '" + var_node->templateName + "'");
            }
            node = prop_it->value->clone();
            break;
        }
        default:
            break;
    }
}

void Analyser::resolve(TemplateUsageNode* node) {
    auto it = m_templates.find(node->name);
    if (it == m_templates.end()) {
        throw std::runtime_error("Unknown template: " + node->name);
    }
    const TemplateNode* templateNode = it->second;

    // This is a placeholder. In a real implementation, you would
    // expand the template here. For now, we'll just check for its existence.
}

void Analyser::resolve(StyleProperty& prop) {
    resolve(prop.value);
}

void Analyser::applySpecializations(std::vector<std::unique_ptr<ASTNode>>& elements, const TemplateUsageNode* usage_node) {
    // Handle deletions first
    if (!usage_node->deleted_elements.empty()) {
        std::vector<bool> to_delete(elements.size(), false);
        for (const auto& del_spec : usage_node->deleted_elements) {
            int current_tag_index = 0;
            for (size_t i = 0; i < elements.size(); ++i) {
                if (elements[i]->getType() == NodeType::Element) {
                    auto* element = static_cast<ElementNode*>(elements[i].get());
                    if (element->tagName == del_spec.tagName) {
                        if (del_spec.index == -1 || del_spec.index == current_tag_index) {
                            to_delete[i] = true;
                            if (del_spec.index != -1) break;
                        }
                        current_tag_index++;
                    }
                }
            }
        }

        auto old_elements = std::move(elements);
        elements.clear();
        for (size_t i = 0; i < old_elements.size(); ++i) {
            if (!to_delete[i]) {
                elements.push_back(std::move(old_elements[i]));
            }
        }
    }

    // Handle insertions
    for (const auto& insertion : usage_node->insertions) {
        std::vector<std::unique_ptr<ASTNode>> new_elements_cloned;
        for (const auto& el : insertion->new_elements) {
            new_elements_cloned.push_back(el->clone());
        }

        if (insertion->position == InsertionPosition::AtTop) {
            elements.insert(elements.begin(), std::make_move_iterator(new_elements_cloned.begin()), std::make_move_iterator(new_elements_cloned.end()));
        } else if (insertion->position == InsertionPosition::AtBottom) {
            elements.insert(elements.end(), std::make_move_iterator(new_elements_cloned.begin()), std::make_move_iterator(new_elements_cloned.end()));
        } else {
            int current_tag_index = 0;
            for (auto it = elements.begin(); it != elements.end(); ++it) {
                if ((*it)->getType() == NodeType::Element) {
                    auto* element = static_cast<ElementNode*>((*it).get());
                    if (element->tagName == insertion->targetTagName) {
                        if (insertion->targetIndex == -1 || insertion->targetIndex == current_tag_index) {
                            if (insertion->position == InsertionPosition::Before) {
                                elements.insert(it, std::make_move_iterator(new_elements_cloned.begin()), std::make_move_iterator(new_elements_cloned.end()));
                            } else if (insertion->position == InsertionPosition::After) {
                                it = elements.insert(it + 1, std::make_move_iterator(new_elements_cloned.begin()), std::make_move_iterator(new_elements_cloned.end()));
                            } else { // Replace
                                it = elements.erase(it);
                                elements.insert(it, std::make_move_iterator(new_elements_cloned.begin()), std::make_move_iterator(new_elements_cloned.end()));
                            }
                            if (insertion->targetIndex != -1) break;
                        }
                        current_tag_index++;
                    }
                }
            }
        }
    }

    for (const auto& spec : usage_node->specializations) {
        int current_tag_index = 0;
        for (auto& element_ptr : elements) {
            if (element_ptr->getType() == NodeType::Element) {
                auto* element = static_cast<ElementNode*>(element_ptr.get());
                if (element->tagName == spec->tagName) {
                    if (spec->index == -1 || spec->index == current_tag_index) {
                        // This is the element we want to modify
                        if (spec->style) {
                            if (!element->style) {
                                element->style = std::make_unique<StyleNode>();
                            }
                            // Merge properties. For now, just append.
                            for (auto& prop : spec->style->properties) {
                                element->style->properties.push_back({prop.key, prop.value->clone()});
                            }
                        }
                        if (spec->index != -1) {
                            break; // We found and modified the indexed element, move to next spec
                        }
                    }
                    current_tag_index++;
                }
            }
        }
    }
}


} // namespace CHTL
