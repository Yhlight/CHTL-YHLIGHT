#include "gtest/gtest.h"
#include "Lexer.h"

TEST(LexerTest, EmptyInput) {
    Lexer lexer("");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::EndOfFile);
}

TEST(LexerTest, SimpleIdentifier) {
    Lexer lexer("div");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::Identifier);
    EXPECT_EQ(tokens[0].value, "div");
    EXPECT_EQ(tokens[1].type, TokenType::EndOfFile);
}

TEST(LexerTest, EqualSign) {
    Lexer lexer("=");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::Equal);
    EXPECT_EQ(tokens[0].value, "=");
    EXPECT_EQ(tokens[1].type, TokenType::EndOfFile);
}
