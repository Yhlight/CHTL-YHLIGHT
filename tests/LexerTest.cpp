#include <gtest/gtest.h>
#include "CHTLLexer/Lexer.h"

TEST(LexerTest, HandlesSingleQuotedStrings) {
    CHTL::Lexer lexer("'hello'");
    CHTL::Token token = lexer.getNextToken();
    ASSERT_EQ(token.type, CHTL::TokenType::String);
    ASSERT_EQ(token.value, "hello");
}

TEST(LexerTest, HandlesUnterminatedSingleQuotedStrings) {
    CHTL::Lexer lexer("'hello");
    CHTL::Token token = lexer.getNextToken();
    ASSERT_EQ(token.type, CHTL::TokenType::Unknown);
}

TEST(LexerTest, HandlesDoubleQuotedStrings) {
    CHTL::Lexer lexer("\"world\"");
    CHTL::Token token = lexer.getNextToken();
    ASSERT_EQ(token.type, CHTL::TokenType::String);
    ASSERT_EQ(token.value, "world");
}

TEST(LexerTest, HandlesSymbols) {
    CHTL::Lexer lexer("{}:;");
    CHTL::Token token1 = lexer.getNextToken();
    ASSERT_EQ(token1.type, CHTL::TokenType::OpenBrace);
    ASSERT_EQ(token1.value, "{");

    CHTL::Token token2 = lexer.getNextToken();
    ASSERT_EQ(token2.type, CHTL::TokenType::CloseBrace);
    ASSERT_EQ(token2.value, "}");

    CHTL::Token token3 = lexer.getNextToken();
    ASSERT_EQ(token3.type, CHTL::TokenType::Colon);
    ASSERT_EQ(token3.value, ":");

    CHTL::Token token4 = lexer.getNextToken();
    ASSERT_EQ(token4.type, CHTL::TokenType::Semicolon);
    ASSERT_EQ(token4.value, ";");
}

TEST(LexerTest, HandlesIdentifiersAndUnquotedLiterals) {
    CHTL::Lexer lexer("div color red");
    CHTL::Token token1 = lexer.getNextToken();
    ASSERT_EQ(token1.type, CHTL::TokenType::Identifier);
    ASSERT_EQ(token1.value, "div");

    CHTL::Token token2 = lexer.getNextToken();
    ASSERT_EQ(token2.type, CHTL::TokenType::Identifier);
    ASSERT_EQ(token2.value, "color");

    CHTL::Token token3 = lexer.getNextToken();
    ASSERT_EQ(token3.type, CHTL::TokenType::Identifier);
    ASSERT_EQ(token3.value, "red");
}

TEST(LexerTest, HandlesComments) {
    CHTL::Lexer lexer("// this is a comment\ndiv /* multi\nline\ncomment */ {");
    CHTL::Token token1 = lexer.getNextToken();
    ASSERT_EQ(token1.type, CHTL::TokenType::Identifier);
    ASSERT_EQ(token1.value, "div");

    CHTL::Token token2 = lexer.getNextToken();
    ASSERT_EQ(token2.type, CHTL::TokenType::OpenBrace);
    ASSERT_EQ(token2.value, "{");
}

TEST(LexerTest, HandlesEqualSign) {
    CHTL::Lexer lexer("=");
    CHTL::Token token = lexer.getNextToken();
    ASSERT_EQ(token.type, CHTL::TokenType::Equal);
    ASSERT_EQ(token.value, "=");
}

TEST(LexerTest, HandlesAlphanumericIdentifiers) {
    CHTL::Lexer lexer("h1 .p2");
    CHTL::Token token1 = lexer.getNextToken();
    ASSERT_EQ(token1.type, CHTL::TokenType::Identifier);
    ASSERT_EQ(token1.value, "h1");

    CHTL::Token token2 = lexer.getNextToken();
    ASSERT_EQ(token2.type, CHTL::TokenType::Identifier);
    ASSERT_EQ(token2.value, ".p2");
}

TEST(LexerTest, HandlesEscapedStrings) {
    CHTL::Lexer lexer(R"('hello \'world\'' "foo \"bar\"")");
    CHTL::Token token1 = lexer.getNextToken();
    ASSERT_EQ(token1.type, CHTL::TokenType::String);
    ASSERT_EQ(token1.value, "hello 'world'");

    CHTL::Token token2 = lexer.getNextToken();
    ASSERT_EQ(token2.type, CHTL::TokenType::String);
    ASSERT_EQ(token2.value, "foo \"bar\"");
}

TEST(LexerTest, HandlesOriginSymbols) {
    CHTL::Lexer lexer("[]@");
    CHTL::Token token1 = lexer.getNextToken();
    ASSERT_EQ(token1.type, CHTL::TokenType::OpenBracket);
    ASSERT_EQ(token1.value, "[");

    CHTL::Token token2 = lexer.getNextToken();
    ASSERT_EQ(token2.type, CHTL::TokenType::CloseBracket);
    ASSERT_EQ(token2.value, "]");

    CHTL::Token token3 = lexer.getNextToken();
    ASSERT_EQ(token3.type, CHTL::TokenType::At);
    ASSERT_EQ(token3.value, "@");
}

TEST(LexerTest, HandlesParentheses) {
    CHTL::Lexer lexer("()");
    CHTL::Token token1 = lexer.getNextToken();
    ASSERT_EQ(token1.type, CHTL::TokenType::OpenParen);
    ASSERT_EQ(token1.value, "(");

    CHTL::Token token2 = lexer.getNextToken();
    ASSERT_EQ(token2.type, CHTL::TokenType::CloseParen);
    ASSERT_EQ(token2.value, ")");
}

TEST(LexerTest, HandlesArithmeticOperators) {
    CHTL::Lexer lexer("+ - * / % **");
    CHTL::Token token1 = lexer.getNextToken();
    ASSERT_EQ(token1.type, CHTL::TokenType::Plus);
    ASSERT_EQ(token1.value, "+");

    CHTL::Token token2 = lexer.getNextToken();
    ASSERT_EQ(token2.type, CHTL::TokenType::Minus);
    ASSERT_EQ(token2.value, "-");

    CHTL::Token token3 = lexer.getNextToken();
    ASSERT_EQ(token3.type, CHTL::TokenType::Asterisk);
    ASSERT_EQ(token3.value, "*");

    CHTL::Token token4 = lexer.getNextToken();
    ASSERT_EQ(token4.type, CHTL::TokenType::Slash);
    ASSERT_EQ(token4.value, "/");

    CHTL::Token token5 = lexer.getNextToken();
    ASSERT_EQ(token5.type, CHTL::TokenType::Percent);
    ASSERT_EQ(token5.value, "%");

    CHTL::Token token6 = lexer.getNextToken();
    ASSERT_EQ(token6.type, CHTL::TokenType::DoubleAsterisk);
    ASSERT_EQ(token6.value, "**");
}
