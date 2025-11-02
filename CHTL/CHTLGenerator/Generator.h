#pragma once

#include "CHTLNode/BaseNode.h"
#include <string>

class Generator {
public:
    explicit Generator(AstNodeList ast);
    std::string generate();

private:
    AstNodeList ast;
    std::string html_output;
    std::string css_output;
    std::string js_output;

    void visit(BaseNode* node);
    void visitElement(class ElementNode* node);
    void visitText(class TextNode* node);
    void visitComment(class CommentNode* node);
    void visitStyle(class StyleNode* node);
    void visitScript(class ScriptNode* node);
};
