#include "gtest/gtest.h"
#include "CHTLJS/CHTLLexer/Lexer.h"
#include "CHTLJS/CHTLParser/Parser.h"
#include "CHTLJS/CHTLGenerator/Generator.h"

std::string compileCHTLJS(const std::string& source) {
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto ast = parser.parse();
    CHTLJS::Generator generator(*ast);
    return generator.generate();
}

TEST(CHTLJSGeneratorTest, GeneratesLiteral) {
    std::string source = "123";
    std::string expected = "123";
    EXPECT_EQ(compileCHTLJS(source), expected);
}

TEST(CHTLJSGeneratorTest, GeneratesSelector) {
    std::string source = "{{mySelector}}";
    std::string expected = "document.querySelector(\"mySelector\")";
    EXPECT_EQ(compileCHTLJS(source), expected);
}

TEST(CHTLJSGeneratorTest, GeneratesBinaryExpression) {
    std::string source = "1 + 2";
    std::string expected = "1 + 2";
    EXPECT_EQ(compileCHTLJS(source), expected);
}

TEST(CHTLJSGeneratorTest, HandlesPrecedence) {
    std::string source = "1 + 2 * 3";
    std::string expected = "1 + 2 * 3";
    EXPECT_EQ(compileCHTLJS(source), expected);
}
