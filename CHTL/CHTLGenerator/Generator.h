#pragma once

#pragma once

#include "CHTLNode/BaseNode.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/ElementDirectiveNode.h"
#include "CHTLNode/CustomNode.h"
#include "CHTLJS/CHTLJSLexer/Lexer.h"
#include "CHTLJS/CHTLJSParser/Parser.h"
#include "CHTLJS/CHTLJSGenerator/Generator.h"
#include <string>
#include <map>

class Generator {
public:
    explicit Generator(AstNodeList ast);
    std::string generate();

private:
    AstNodeList ast;
    std::string html_output;
    std::string css_output;
    std::string js_output;
    std::map<std::string, std::string> style_templates;
    std::map<std::string, TemplateNode*> element_templates;
    std::map<std::string, std::map<std::string, std::string>> variable_templates;
    std::map<std::string, CustomNode*> custom_style_templates;

    void visit(BaseNode* node);
    void visitElement(class ElementNode* node);
    void visitText(class TextNode* node);
    void visitComment(class CommentNode* node);
    void visitStyle(class StyleNode* node);
    void visitScript(class ScriptNode* node);
    void visitTemplate(class TemplateNode* node);
    void visitElementDirective(class ElementDirectiveNode* node);
    void visitCustom(class CustomNode* node);
};
