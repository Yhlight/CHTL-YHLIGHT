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

TEST(CHTLJSGeneratorTest, GeneratesTagSelector) {
    std::string source = "{{mySelector}}";
    std::string expected = "document.querySelectorAll(\"mySelector\")";
    EXPECT_EQ(compileCHTLJS(source), expected);
}

TEST(CHTLJSGeneratorTest, GeneratesIdSelector) {
    std::string source = "{{#myId}}";
    std::string expected = "document.getElementById(\"myId\")";
    EXPECT_EQ(compileCHTLJS(source), expected);
}

TEST(CHTLJSGeneratorTest, GeneratesClassSelector) {
    std::string source = "{{.myClass}}";
    std::string expected = "document.querySelectorAll(\".myClass\")";
    EXPECT_EQ(compileCHTLJS(source), expected);
}

TEST(CHTLJSGeneratorTest, GeneratesCompoundSelector) {
    std::string source = "{{.myClass myDescendant}}";
    std::string expected = "document.querySelectorAll(\".myClass myDescendant\")";
    EXPECT_EQ(compileCHTLJS(source), expected);
}

TEST(CHTLJSGeneratorTest, GeneratesIndexedSelector) {
    std::string source = "{{myTag[0]}}";
    std::string expected = "document.querySelectorAll(\"myTag\")[0]";
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
