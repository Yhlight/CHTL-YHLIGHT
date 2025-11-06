#include <gtest/gtest.h>
#include "CHTLJS/CHTLJSLexer/Lexer.h"
#include "CHTLJS/CHTLJSParser/Parser.h"
#include "CHTLJS/CHTLJSGenerator/Generator.h"
#include "CHTLJS/CHTLJSNode/ProgramNode.h"

using namespace CHTLJS;

TEST(CHTLJSGeneratorTest, GenerateSimpleSelector) {
    std::string source = "{{#my-id}};";
    Lexer lexer(source);
    Parser parser(lexer);
    Generator generator;

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    std::string result = generator.generate(*program);
    EXPECT_EQ(result, "document.querySelector('#my-id')");
}

TEST(CHTLJSGeneratorTest, GenerateComplexSelector) {
    std::string source = "{{.box button[0]}};";
    Lexer lexer(source);
    Parser parser(lexer);
    Generator generator;

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    std::string result = generator.generate(*program);
    EXPECT_EQ(result, "document.querySelectorAll('.box button')[0]");
}

TEST(CHTLJSGeneratorTest, GenerateCallExpression) {
    std::string source = "{{myObject}}->myMethod();";
    Lexer lexer(source);
    Parser parser(lexer);
    Generator generator;

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    std::string result = generator.generate(*program);
    EXPECT_EQ(result, "document.querySelector('myObject').myMethod()");
}
