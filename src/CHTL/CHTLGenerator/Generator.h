#pragma once

#include "BaseNode.h"
#include <string>
#include <sstream>
#include <map>
#include <memory>

class ElementNode;
class TextNode;
class StyleNode;
class StylePropertyNode;
class StyleRuleNode;
class ScriptNode;
class TemplateNode;
class CustomNode;
class ElementDirectiveNode;
class StyleDirectiveNode;
class ProgramNode;
class OriginNode;
class OriginDirectiveNode;
class ImportNode;
class NamespaceNode;

class Generator {
public:
    Generator(const BaseNode& root);
    std::string generate(bool full_document = true);

private:
    void visit(const BaseNode* node);
    void visit(const ProgramNode* node);
    void visit(const ElementNode* node);
    void visit(const TextNode* node);
    void visit(const ScriptNode* node);
    void visit(const TemplateNode* node);
    void visit(const CustomNode* node);
    void visit(const ElementDirectiveNode* node);
    std::string visit(const StyleDirectiveNode* node);
    void visit(const OriginNode* node);
    void visit(const OriginDirectiveNode* node);
    void visit(const ImportNode* node);
    void visit(const NamespaceNode* node);

    const BaseNode& root;
    std::stringstream html_output;
    std::stringstream css_output;
    std::stringstream js_output;
    std::string current_namespace;
    std::map<std::string, std::map<std::string, const TemplateNode*>> element_templates;
    std::map<std::string, std::map<std::string, const TemplateNode*>> style_templates;
    std::map<std::string, std::map<std::string, const TemplateNode*>> var_templates;
    std::map<std::string, std::map<std::string, const CustomNode*>> custom_style_templates;
    std::map<std::string, std::unique_ptr<OriginNode>> named_origin_blocks;
};
