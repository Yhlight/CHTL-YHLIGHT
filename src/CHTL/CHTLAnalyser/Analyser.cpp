#include "Analyser.h"
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <iterator>
#include <algorithm>
#include <sstream>


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
    for (size_t i = 0; i < node->children.size();) {
        visit(node->children[i].get());
        // After visiting, if the node was an import, it might have been replaced.
        // Re-evaluate the size and current position.
        if (node->children[i]->getType() != NodeType::Import) {
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

    for (auto& child : node->children) {
        visit(child.get());
    }
}

void Analyser::visit(TemplateNode* node) {
    if (m_templates.count(node->name)) {
        throw std::runtime_error("Template with name '" + node->name + "' already defined.");
    }
    m_templates[node->name] = node;
}

void Analyser::visit(NamespaceNode* node) {
    m_symbol_table.pushNamespace(node->name);
    for (auto& child : node->children) {
        visit(child.get());
    }
    m_symbol_table.popNamespace();
}

void Analyser::visit(ImportNode* node) {
    if (node->importType != ImportType::Chtl) {
        std::string content = m_importer.importRawFile(node->filePath, m_filePath);
        auto originNode = std::make_unique<OriginNode>();
        originNode->content = std::move(content);

        switch (node->importType) {
            case ImportType::Html:
                originNode->type = "Html";
                break;
            case ImportType::Style:
                originNode->type = "Style";
                break;
            case ImportType::JavaScript:
                originNode->type = "JavaScript";
                break;
            default:
                // Should not happen due to parser validation
                break;
        }

        // Find the import node in the main program and replace it
        auto& children = m_program.children;
        auto it = std::find_if(children.begin(), children.end(),
                               [node](const std::unique_ptr<ASTNode>& child) {
                                   return child.get() == node;
                               });

        if (it != children.end()) {
            *it = std::move(originNode);
        }

    } else {
		std::filesystem::path canonical_path = std::filesystem::weakly_canonical(std::filesystem::path(m_filePath).parent_path() / node->filePath);
		std::string canonical_path_str = canonical_path.string();

		if (m_import_stack.count(canonical_path_str)) {
			throw std::runtime_error("Circular import detected: " + node->filePath);
		}
		m_import_stack.insert(canonical_path_str);

		auto imported_ast = m_importer.importFile(node->filePath, m_filePath);
		auto* imported_program = static_cast<ProgramNode*>(imported_ast.get());

		// Create a new analyser for the imported file and run it.
		Analyser imported_analyser(*imported_program, canonical_path_str);
		imported_analyser.m_import_stack = this->m_import_stack; // Share the import stack
		imported_analyser.analyse();

		// Find the import node in the main program and replace it
		auto& children = m_program.children;
		auto it = std::find_if(children.begin(), children.end(),
			[node](const std::unique_ptr<ASTNode>& child) {
				return child.get() == node;
			});

		if (it != children.end()) {
			it = children.erase(it);
			children.insert(it,
				std::make_move_iterator(imported_program->children.begin()),
				std::make_move_iterator(imported_program->children.end()));
		}

		m_import_stack.erase(canonical_path_str);
	}
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
    std::vector<std::unique_ptr<ASTNode>> new_children;
    for (auto& child : node->children) {
        if (child->getType() == NodeType::TemplateUsage) {
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
                for (auto& expanded_child : expanded_children) {
                    new_children.push_back(std::move(expanded_child));
                }
            } else {
                new_children.push_back(std::move(child));
            }
        } else if (child->getType() == NodeType::Namespace) {
            auto* ns_node = static_cast<NamespaceNode*>(child.get());
            m_symbol_table.pushNamespace(ns_node->name);
            for (auto& ns_child : ns_node->children) {
                resolve(ns_child.get());
                new_children.push_back(std::move(ns_child));
            }
            m_symbol_table.popNamespace();
        } else {
            resolve(child.get());
            new_children.push_back(std::move(child));
        }
    }
    node->children = std::move(new_children);
}

void Analyser::resolve(ElementNode* node) {
    if (node->style) {
        resolve(node->style.get());
    }

    std::vector<std::unique_ptr<ASTNode>> new_children;
    for (auto& child : node->children) {
        if (child->getType() == NodeType::TemplateUsage) {
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
                for (auto& expanded_child : expanded_children) {
                    new_children.push_back(std::move(expanded_child));
                }
            } else {
                 new_children.push_back(std::move(child));
            }
        } else {
            resolve(child.get());
            new_children.push_back(std::move(child));
        }
    }
    node->children = std::move(new_children);
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
