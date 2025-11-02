#include "Test.h"
#include <stdexcept>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/ScriptNode.h"
#include "CHTLNode/CommentNode.h"

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
        Parser parser(tokens);
        AstNode ast = parser.parse();

        ASSERT(ast != nullptr);
        ElementNode* root = static_cast<ElementNode*>(ast.get());
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
        Parser parser(tokens);
        AstNode ast = parser.parse();

        ASSERT(ast != nullptr);
        ElementNode* body = static_cast<ElementNode*>(ast.get());
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
        ASSERT(style->content.find("color") != std::string::npos);

        ScriptNode* script = static_cast<ScriptNode*>(body->children[2].get());
        ASSERT(script->content.find("let x = 1") != std::string::npos);
    }});
}
