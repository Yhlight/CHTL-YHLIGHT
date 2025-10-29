#include "Generator.h"
#include <algorithm>
#include <vector>
#include <unordered_set>

namespace CHTL {

Generator::Generator(const ProgramNode& program) : m_program(program) {}

std::string Generator::generate() {
    collectStyles(&m_program);
    visit(&m_program);
    return m_output.str();
}

void Generator::visit(const ASTNode* node) {
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
             // Style nodes are handled within element nodes, so we don't visit them directly.
            break;
        case NodeType::Origin:
            visit(static_cast<const OriginNode*>(node));
            break;
    }
}

void Generator::visit(const OriginNode* node) {
    indent();
    if (node->type == ImportType::Css) {
        m_output << "<style>\n" << node->content << "\n</style>\n";
    } else if (node->type == ImportType::JavaScript) {
        m_output << "<script>\n" << node->content << "\n</script>\n";
    } else if (node->type == ImportType::Html) {
        m_output << node->content << "\n";
    }
}

void Generator::visit(const ProgramNode* node) {
    // A simplified approach: we assume a single top-level `html` element.
    // A more robust generator would handle fragments or multiple top-level elements.
    if (node->children.size() == 1 && node->children[0]->getType() == NodeType::Element) {
        const auto* html_node = static_cast<const ElementNode*>(node->children[0].get());
        if (html_node->tagName == "html") {
             m_output << "<html>\n";
             // Find head and inject styles
             bool head_found = false;
             for(const auto& child : html_node->children){
                 if(child->getType() == NodeType::Element){
                     const auto* element = static_cast<const ElementNode*>(child.get());
                     if(element->tagName == "head"){
                         head_found = true;
                     }
                 }
             }

            if(!head_found && !m_global_styles.empty()){
                m_output << "<head>\n<style>\n";
                for (const auto& style : m_global_styles) {
                    m_output << style;
                }
                m_output << "</style>\n</head>\n";
            }

            for(const auto& child : html_node->children){
                visit(child.get());
            }

            m_output << "</html>\n";
            return;
        }
    }

    // Fallback for non-standard structure
    for (const auto& child : node->children) {
        visit(child.get());
    }
}

void Generator::visit(const ElementNode* node) {
    if(node->tagName == "head" && !m_global_styles.empty()){
        indent();
        m_output << "<head>\n";
        m_indent_level++;
        indent();
        m_output << "<style>\n";
        for (const auto& style : m_global_styles) {
            m_output << style;
        }
        m_output << "</style>\n";

        for (const auto& child : node->children) {
            visit(child.get());
        }

        m_indent_level--;
        indent();
        m_output << "</head>\n";
        return;
    }


    indent();
    m_output << "<" << node->tagName;

    bool has_class = false;
    bool has_id = false;
    for (const auto& attr : node->attributes) {
        if (attr.key == "class") has_class = true;
        if (attr.key == "id") has_id = true;
        m_output << " " << attr.key << "=\"" << attr.value << "\"";
    }

    if (!has_class && !node->auto_classes.empty()) {
        m_output << " class=\"" << node->auto_classes[0] << "\"";
    }
    if (!has_id && !node->auto_ids.empty()) {
        m_output << " id=\"" << node->auto_ids[0] << "\"";
    }

    if (node->style && !node->style->properties.empty()) {
        m_output << " style=\"";
        std::vector<const StyleProperty*> props_to_generate;
        std::unordered_set<std::string> keys;

        for (int i = node->style->properties.size() - 1; i >= 0; --i) {
            const auto& prop = node->style->properties[i];
            if (keys.find(prop.key) == keys.end()) {
                props_to_generate.push_back(&prop);
                keys.insert(prop.key);
            }
        }

        std::reverse(props_to_generate.begin(), props_to_generate.end());

        for (size_t i = 0; i < props_to_generate.size(); ++i) {
            const auto* prop = props_to_generate[i];
            m_output << prop->key << ": " << m_evaluator.evaluate(prop->value.get()) << ";";
            if (i < props_to_generate.size() - 1) {
                m_output << " ";
            }
        }
        m_output << "\"";
    }


    const static std::unordered_set<std::string> self_closing_tags = {
        "area", "base", "br", "col", "embed", "hr", "img", "input",
        "link", "meta", "param", "source", "track", "wbr"
    };

    if (self_closing_tags.count(node->tagName)) {
        m_output << " />\n";
        return;
    }

    if (node->children.empty()) {
        m_output << "></" << node->tagName << ">\n";
        return;
    }

    m_output << ">\n";
    m_indent_level++;

    for (const auto& child : node->children) {
        visit(child.get());
    }

    m_indent_level--;
    indent();
    m_output << "</" << node->tagName << ">\n";
}

void Generator::visit(const TextNode* node) {
    indent();
    m_output << node->content << "\n";
}

void Generator::indent() {
    for (int i = 0; i < m_indent_level; ++i) {
        m_output << "  "; // 2 spaces for indentation
    }
}


void Generator::collectStyles(const ASTNode* node) {
    if (!node) return;

    if (node->getType() == NodeType::Element) {
        const auto* element = static_cast<const ElementNode*>(node);
        if (element->style) {
            std::string base_selector;
            if (!element->auto_ids.empty()) {
                base_selector = "#" + element->auto_ids[0];
            } else if (!element->auto_classes.empty()) {
                base_selector = "." + element->auto_classes[0];
            } else if (!element->attributes.empty()) {
                 for(const auto& attr : element->attributes){
                    if(attr.key == "id"){
                        base_selector = "#" + attr.value;
                        break;
                    }
                    if(attr.key == "class"){
                        base_selector = "." + attr.value;
                        break;
                    }
                }
            }


            for (const auto& block : element->style->selector_blocks) {
                std::string final_selector = block->selector;
                // Replace '&' with the base selector
                size_t pos = final_selector.find('&');
                if (pos != std::string::npos) {
                    final_selector.replace(pos, 1, base_selector);
                }

                std::stringstream ss;
                ss << final_selector << " {\n";
                for (const auto& prop : block->properties) {
                    ss << "  " << prop.key << ": " << m_evaluator.evaluate(prop.value.get()) << ";\n";
                }
                ss << "}\n";
                m_global_styles.push_back(ss.str());
            }
        }

        for (const auto& child : element->children) {
            collectStyles(child.get());
        }
    } else if (node->getType() == NodeType::Program) {
        const auto* program = static_cast<const ProgramNode*>(node);
        for (const auto& child : program->children) {
            collectStyles(child.get());
        }
    }
}

} // namespace CHTL
