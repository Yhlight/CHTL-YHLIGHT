#include <gtest/gtest.h>
#include "CHTLJS/CHTLJSLexer/Lexer.h"
#include "CHTLJS/CHTLJSParser/Parser.h"
#include "CHTLJS/CHTLJSGenerator/CHTLJSGenerator.h"
#include "CHTLJS/CHTLJSNode/ProgramNode.h"

using namespace CHTLJS;

TEST(CHTLJSGeneratorTest, GenerateSimpleSelector) {
    std::string source = "{{#my-id}}";
    Lexer lexer(source);
    Parser parser(lexer);
    CHTLJSGenerator generator;

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    std::string result = generator.generate(*program);
    EXPECT_EQ(result, "document.querySelector('#my-id');\n");
}

TEST(CHTLJSGeneratorTest, GenerateComplexSelector) {
    std::string source = "{{.box button[0]}}";
    Lexer lexer(source);
    Parser parser(lexer);
    CHTLJSGenerator generator;

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    std::string result = generator.generate(*program);
    EXPECT_EQ(result, "document.querySelectorAll('.box button')[0];\n");
}

TEST(CHTLJSGeneratorTest, GenerateCallExpression) {
    std::string source = "{{myObject}}->myMethod()";
    Lexer lexer(source);
    Parser parser(lexer);
    CHTLJSGenerator generator;

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    std::string result = generator.generate(*program);
    EXPECT_EQ(result, "document.querySelector('myObject').myMethod();\n");
}

TEST(CHTLJSGeneratorTest, GenerateLiteral) {
    std::string source = "\"hello\"";
    Lexer lexer(source);
    Parser parser(lexer);
    CHTLJSGenerator generator;

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    std::string result = generator.generate(*program);
    EXPECT_EQ(result, "\"hello\";\n");
}

TEST(CHTLJSGeneratorTest, GenerateBinaryExpression) {
    std::string source = "1 + 2";
    Lexer lexer(source);
    Parser parser(lexer);
    CHTLJSGenerator generator;

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    std::string result = generator.generate(*program);
    EXPECT_EQ(result, "1 + 2;\n");
}

TEST(CHTLJSGeneratorTest, GenerateUnaryExpression) {
    std::string source = "!x";
    Lexer lexer(source);
    Parser parser(lexer);
    CHTLJSGenerator generator;

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    std::string result = generator.generate(*program);
    EXPECT_EQ(result, "!x;\n");
}

TEST(CHTLJSGeneratorTest, GenerateMemberAccessExpression) {
    std::string source = "{{myObject}}->myMember";
    Lexer lexer(source);
    Parser parser(lexer);
    CHTLJSGenerator generator;

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    std::string result = generator.generate(*program);
    EXPECT_EQ(result, "document.querySelector('myObject').myMember;\n");
}
