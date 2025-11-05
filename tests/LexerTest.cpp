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

TEST(LexerTest, Dot) {
    Lexer lexer(".");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::Dot);
    EXPECT_EQ(tokens[0].value, ".");
    EXPECT_EQ(tokens[1].type, TokenType::EndOfFile);
}

TEST(LexerTest, At) {
    Lexer lexer("@");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::At);
    EXPECT_EQ(tokens[0].value, "@");
    EXPECT_EQ(tokens[1].type, TokenType::EndOfFile);
}

TEST(LexerTest, TemplateKeyword) {
    Lexer lexer("[Template]");
    auto tokens = lexer.tokenize();
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::TemplateKeyword);
    EXPECT_EQ(tokens[0].value, "[Template]");
    EXPECT_EQ(tokens[1].type, TokenType::EndOfFile);
}
