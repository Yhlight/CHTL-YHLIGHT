#include "Test.h"
#include <stdexcept>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"

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
}
