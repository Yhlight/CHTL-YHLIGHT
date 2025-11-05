#pragma once

#include "BaseNode.h"
#include <string>
#include <sstream>

class ElementNode;
class TextNode;

class Generator {
public:
    Generator(const BaseNode& root);
    std::string generate();

private:
    void visit(const BaseNode* node);
    void visit(const ElementNode* node);
    void visit(const TextNode* node);

    const BaseNode& root;
    std::stringstream output;
};
