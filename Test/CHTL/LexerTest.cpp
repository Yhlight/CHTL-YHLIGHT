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
    EXPECT_EQ(token.type, CHTL::TokenType::Colon);
    EXPECT_EQ(token.value, "=");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::EndOfFile);
}

TEST(LexerTest, UnquotedLiteralInAttributeWithEquals) {
    std::string source = "id = box;";
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

TEST(LexerTest, SingleQuotedStringLiteral) {
    std::string source = "'hello world'";
    CHTL::Lexer lexer(source);

    CHTL::Token token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::String);
    EXPECT_EQ(token.value, "hello world");

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

TEST(LexerTest, Comments) {
    std::string source = R"(
        // This is a line comment.
        div {
            /**
             * This is a block comment.
             */
            # This is a generator comment.
        }
    )";
    CHTL::Lexer lexer(source);

    CHTL::Token token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Identifier);
    EXPECT_EQ(token.value, "div");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::OpenBrace);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::GeneratorComment);
    EXPECT_EQ(token.value, "# This is a generator comment.");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::CloseBrace);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::EndOfFile);
}

TEST(LexerTest, TemplateStyleDefinition) {
    std::string source = "[Template] @Style MyStyles {}";
    CHTL::Lexer lexer(source);

    CHTL::Token token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::TemplateKeyword);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Identifier);
    EXPECT_EQ(token.value, "@Style");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Identifier);
    EXPECT_EQ(token.value, "MyStyles");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::OpenBrace);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::CloseBrace);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::EndOfFile);
}

TEST(LexerTest, LexRawContent) {
    std::string source = "[Origin] @Html {<div><span>{nested}</span></div>}";
    CHTL::Lexer lexer(source);

    CHTL::Token token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::OriginKeyword);

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::Identifier);
    EXPECT_EQ(token.value, "@Html");

    token = lexer.nextToken();
    EXPECT_EQ(token.type, CHTL::TokenType::RAW_CONTENT);
    EXPECT_EQ(token.value, "<div><span>{nested}</span></div>");

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
