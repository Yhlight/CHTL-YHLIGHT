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
class ElementDirectiveNode;
class StyleDirectiveNode;
class ProgramNode;

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
    void visit(const ElementDirectiveNode* node);
    void visit(const StyleDirectiveNode* node);

    const BaseNode& root;
    std::stringstream html_output;
    std::stringstream css_output;
    std::stringstream js_output;
    std::map<std::string, const TemplateNode*> element_templates;
    std::map<std::string, const TemplateNode*> style_templates;
    std::map<std::string, const TemplateNode*> var_templates;
};
