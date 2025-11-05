#pragma once

#include "BaseNode.h"
#include <string>
#include <sstream>

class ElementNode;
class TextNode;
class StyleNode;
class StylePropertyNode;
class StyleRuleNode;
class ScriptNode;

class Generator {
public:
    Generator(const BaseNode& root);
    std::string generate(bool full_document = true);

private:
    void visit(const BaseNode* node);
    void visit(const ElementNode* node);
    void visit(const TextNode* node);
    void visit(const ScriptNode* node);

    const BaseNode& root;
    std::stringstream html_output;
    std::stringstream css_output;
    std::stringstream js_output;
};
