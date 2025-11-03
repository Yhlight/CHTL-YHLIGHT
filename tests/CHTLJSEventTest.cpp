#include "gtest/gtest.h"
#include "CHTLJS/CHTLLexer/Lexer.h"
#include "CHTLJS/CHTLParser/Parser.h"
#include "CHTLJS/CHTLGenerator/Generator.h"
#include "CHTLJS/CHTLNode/ListenNode.h"
#include "CHTLJS/CHTLNode/DelegateNode.h"
#include <vector>
#include <string>

using namespace CHTLJS;

TEST(CHTLJSEventTest, ListenExpression) {
    std::string source = "{{#myButton}} Listen { click: \"() => console.log('clicked')\" }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto ast = parser.parse();
    Generator generator(*ast);
    std::string result = generator.generate();
    std::string expected = "document.getElementById(\"myButton\").addEventListener('click', () => console.log('clicked'));";
    ASSERT_EQ(result, expected);
}

TEST(CHTLJSEventTest, ListenExpressionMultiHandler) {
    std::string source = "{{#myButton}} Listen { click: \"() => console.log('clicked')\", mouseover: \"() => console.log('hovered')\" }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto ast = parser.parse();
    Generator generator(*ast);
    std::string result = generator.generate();
    std::string expected = "document.getElementById(\"myButton\").addEventListener('click', () => console.log('clicked'));\ndocument.getElementById(\"myButton\").addEventListener('mouseover', () => console.log('hovered'));";
    ASSERT_EQ(result, expected);
}

TEST(CHTLJSEventTest, DelegateExpression) {
    std::string source = "{{#myList}} Delegate { target: {{.item}}, click: \"() => console.log('item clicked')\" }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto ast = parser.parse();
    Generator generator(*ast);
    std::string result = generator.generate();
    std::string expected = "document.getElementById(\"myList\").addEventListener('click', event => {\n  if (event.target.matches('.item')) {\n    (() => console.log('item clicked'))(event);\n  }\n});";
    ASSERT_EQ(result, expected);
}
