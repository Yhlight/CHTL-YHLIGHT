#include "Test.h"
#include <stdexcept>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/ScriptNode.h"
#include "CHTLNode/CommentNode.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/StyleContentNode.h"
#include "CHTLNode/CustomNode.h"
#include "CHTLNode/OriginNode.h"
#include "CHTLNode/ImportNode.h"
#include "CHTLNode/NamespaceNode.h"
#include "CHTLNode/OriginDirectiveNode.h"

#define ASSERT(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed at " + std::string(__FILE__) + ":" + std::to_string(__LINE__)); \
    }

void ParserTests() {
    tests.push_back({"Test Basic Parsing", []() {
        std::string source = R"(
            div {
                id: "main";
                text {
                    "Hello, CHTL!"
                }
            }
        )";

        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();
        Parser parser(tokens, source);
        AstNodeList ast = parser.parse();

        ASSERT(ast.size() == 1);
        ElementNode* root = static_cast<ElementNode*>(ast[0].get());
        ASSERT(root->tag_name == "div");
        ASSERT(root->attributes.size() == 1);
        ASSERT(root->attributes["id"] == "main");
        ASSERT(root->children.size() == 1);
        TextNode* text = static_cast<TextNode*>(root->children[0].get());
        ASSERT(text->text == "Hello, CHTL!");
    }});

    tests.push_back({"Test Nested Elements and Behaviors", []() {
        std::string source = R"(
            body {
                // line comment
                div {
                    class: "container";
                    h1 {
                        text { "Title" }
                    }
                    style {
                        color: red;
                    }
                }
                script {
                    let x = 1;
                }
            }
        )";

        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();
        Parser parser(tokens, source);
        AstNodeList ast = parser.parse();

        ASSERT(ast.size() > 0);
        ElementNode* body = static_cast<ElementNode*>(ast[0].get());
        ASSERT(body->tag_name == "body");
        ASSERT(body->children.size() == 3);

        CommentNode* comment = static_cast<CommentNode*>(body->children[0].get());
        ASSERT(comment->type == TokenType::LineComment);

        ElementNode* div = static_cast<ElementNode*>(body->children[1].get());
        ASSERT(div->tag_name == "div");
        ASSERT(div->attributes["class"] == "container");
        ASSERT(div->children.size() == 2);

        ElementNode* h1 = static_cast<ElementNode*>(div->children[0].get());
        ASSERT(h1->tag_name == "h1");
        TextNode* h1_text = static_cast<TextNode*>(h1->children[0].get());
        ASSERT(h1_text->text == "Title");

        StyleNode* style = static_cast<StyleNode*>(div->children[1].get());
        ASSERT(!style->content.empty());
        RawStyleContentNode* raw_style = dynamic_cast<RawStyleContentNode*>(style->content[0].get());
        ASSERT(raw_style != nullptr);
        ASSERT(raw_style->raw_css.find("color") != std::string::npos);

        ScriptNode* script = static_cast<ScriptNode*>(body->children[2].get());
        ASSERT(script->content.find("let x = 1") != std::string::npos);
    }});

    tests.push_back({"Test Top-Level Constructs", []() {
        std::string source = R"(
            [Template] @Element MyTemplate {
                div {}
            }
            [Custom] @Style MyCustomStyle {
                span {}
            }
            [Origin] @Html MyOrigin {}
            [Import] @Chtl from "my.chtl"
            [Namespace] MyNamespace {
                p {}
            }
            div {}
        )";

        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();
        Parser parser(tokens, source);
        AstNodeList ast = parser.parse();

        ASSERT(ast.size() == 6);
        TemplateNode* templateNode = dynamic_cast<TemplateNode*>(ast[0].get());
        ASSERT(templateNode != nullptr);
        ASSERT(templateNode->type == "@Element");
        ASSERT(templateNode->name == "MyTemplate");
        ASSERT(templateNode->body.size() > 0);
        ElementNode* div = dynamic_cast<ElementNode*>(templateNode->body[0].get());
        ASSERT(div != nullptr);
        ASSERT(div->tag_name == "div");
        CustomNode* customNode = dynamic_cast<CustomNode*>(ast[1].get());
        ASSERT(customNode != nullptr);
        ASSERT(customNode->type == "@Style");
        ASSERT(customNode->name == "MyCustomStyle");
        ASSERT(customNode->body.size() > 0);
        ElementNode* span = dynamic_cast<ElementNode*>(customNode->body[0].get());
        ASSERT(span != nullptr);
        ASSERT(span->tag_name == "span");
        OriginNode* originNode = dynamic_cast<OriginNode*>(ast[2].get());
        ASSERT(originNode != nullptr);
        ASSERT(originNode->type == "@Html");
        ASSERT(originNode->name == "MyOrigin");
        ImportNode* importNode = dynamic_cast<ImportNode*>(ast[3].get());
        ASSERT(importNode != nullptr);
        ASSERT(importNode->type == "@Chtl");
        ASSERT(importNode->file_path == "my.chtl");
        NamespaceNode* namespaceNode = dynamic_cast<NamespaceNode*>(ast[4].get());
        ASSERT(namespaceNode != nullptr);
        ASSERT(namespaceNode->name == "MyNamespace");
        ASSERT(namespaceNode->body.size() > 0);
        ElementNode* p = dynamic_cast<ElementNode*>(namespaceNode->body[0].get());
        ASSERT(p != nullptr);
        ASSERT(p->tag_name == "p");
        ASSERT(dynamic_cast<ElementNode*>(ast[5].get()) != nullptr);
    }});

    tests.push_back({"Test Origin Directive Parsing", []() {
        std::string source = R"(
            [Origin] @Html myBlock;
        )";

        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();
        Parser parser(tokens, source);
        AstNodeList ast = parser.parse();

        ASSERT(ast.size() == 1);
        OriginDirectiveNode* directive = dynamic_cast<OriginDirectiveNode*>(ast[0].get());
        ASSERT(directive != nullptr);
        ASSERT(directive->type == "@Html");
        ASSERT(directive->name == "myBlock");
    }});
}
