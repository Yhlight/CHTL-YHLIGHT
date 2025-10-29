#include "gtest/gtest.h"
#include "CHTL/Lexer.h"
#include "CHTL/Token.h"

TEST(LexerTest, GeneratorComment) {
    std::string source = "# a comment";
    CHTL::Lexer lexer(source);
    CHTL::Token token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::GeneratorComment);
    EXPECT_EQ(token.value, "# a comment");
}

TEST(LexerTest, NotGeneratorComment) {
    std::string source = "#not_a_comment";
    CHTL::Lexer lexer(source);
    CHTL::Token token = lexer.nextToken();
    EXPECT_NE(token.type, CHTL::TokenType::GeneratorComment);
}
