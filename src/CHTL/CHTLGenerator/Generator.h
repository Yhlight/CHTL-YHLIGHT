#pragma once

#include "BaseNode.h"
#include <string>
#include <sstream>

class ElementNode;
class TextNode;
class StyleNode;
class StylePropertyNode;
class StyleRuleNode;

class Generator {
public:
    Generator(const BaseNode& root);
    std::string generate();

private:
    void visit(const BaseNode* node);
    void visit(const ElementNode* node);
    void visit(const TextNode* node);

    const BaseNode& root;
    std::stringstream html_output;
    std::stringstream css_output;
};
