#include <gtest/gtest.h>
#include "CHTL/Lexer.h"

TEST(LexerTest, SimpleTokens) {
    std::string source = "{};:=";
    CHTL::Lexer lexer(source);

    CHTL::Token token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::OpenBrace);
    EXPECT_EQ(token.value, "{");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::CloseBrace);
    EXPECT_EQ(token.value, "}");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Semicolon);
    EXPECT_EQ(token.value, ";");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Colon);
    EXPECT_EQ(token.value, ":");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Equals);
    EXPECT_EQ(token.value, "=");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::EndOfFile);
}

TEST(LexerTest, UnquotedLiteralInAttribute) {
    std::string source = "id: box;";
    CHTL::Lexer lexer(source);

    CHTL::Token token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Identifier);
    EXPECT_EQ(token.value, "id");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Colon);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Identifier);
    EXPECT_EQ(token.value, "box");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Semicolon);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::EndOfFile);
}

TEST(LexerTest, StringLiteral) {
    std::string source = "\"hello world\"";
    CHTL::Lexer lexer(source);

    CHTL::Token token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::String);
    EXPECT_EQ(token.value, "hello world");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::EndOfFile);
}

TEST(LexerTest, Identifier) {
    std::string source = "div";
    CHTL::Lexer lexer(source);

    CHTL::Token token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Identifier);
    EXPECT_EQ(token.value, "div");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::EndOfFile);
}
