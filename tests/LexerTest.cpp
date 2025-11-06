#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLLexer/Token.h"

TEST(LexerTest, SingleCharacterTokens) {
    std::string source = "(){}[];:=";
    Lexer lexer(source);

    std::vector<TokenType> expected_tokens = {
        TokenType::LEFT_PAREN,
        TokenType::RIGHT_PAREN,
        TokenType::LEFT_BRACE,
        TokenType::RIGHT_BRACE,
        TokenType::LEFT_BRACKET,
        TokenType::RIGHT_BRACKET,
        TokenType::SEMICOLON,
        TokenType::COLON,
        TokenType::EQUAL,
        TokenType::END_OF_FILE
    };

    for (const auto& expected_token : expected_tokens) {
        Token token = lexer.scanToken();
        EXPECT_EQ(token.type, expected_token);
    }
}

TEST(LexerTest, Keywords) {
    std::string source = "style script text";
    Lexer lexer(source);

    std::vector<TokenType> expected_tokens = {
        TokenType::KEYWORD_STYLE,
        TokenType::KEYWORD_SCRIPT,
        TokenType::KEYWORD_TEXT,
        TokenType::END_OF_FILE
    };

    for (const auto& expected_token : expected_tokens) {
        Token token = lexer.scanToken();
        EXPECT_EQ(token.type, expected_token);
    }
}

TEST(LexerTest, Identifier) {
    std::string source = "my_var anothervar";
    Lexer lexer(source);

    Token token1 = lexer.scanToken();
    EXPECT_EQ(token1.type, TokenType::IDENTIFIER);
    EXPECT_EQ(token1.lexeme, "my_var");

    Token token2 = lexer.scanToken();
    EXPECT_EQ(token2.type, TokenType::IDENTIFIER);
    EXPECT_EQ(token2.lexeme, "anothervar");
}

TEST(LexerTest, StringLiteral) {
    std::string source = "'hello' \"world\"";
    Lexer lexer(source);

    Token token1 = lexer.scanToken();
    EXPECT_EQ(token1.type, TokenType::STRING);
    EXPECT_EQ(token1.lexeme, "'hello'");

    Token token2 = lexer.scanToken();
    EXPECT_EQ(token2.type, TokenType::STRING);
    EXPECT_EQ(token2.lexeme, "\"world\"");
}

TEST(LexerTest, NumberLiteral) {
    std::string source = "123 45.67";
    Lexer lexer(source);

    Token token1 = lexer.scanToken();
    EXPECT_EQ(token1.type, TokenType::NUMBER);
    EXPECT_EQ(token1.lexeme, "123");

    Token token2 = lexer.scanToken();
    EXPECT_EQ(token2.type, TokenType::NUMBER);
    EXPECT_EQ(token2.lexeme, "45.67");
}

TEST(LexerTest, Comments) {
    std::string source = R"(
        // this is a comment
        style // another comment
        /* block comment */
        text
    )";
    Lexer lexer(source);

    Token token1 = lexer.scanToken();
    EXPECT_EQ(token1.type, TokenType::KEYWORD_STYLE);

    Token token2 = lexer.scanToken();
    EXPECT_EQ(token2.type, TokenType::KEYWORD_TEXT);
}
