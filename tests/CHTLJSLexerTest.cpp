#include <gtest/gtest.h>
#include "CHTLJS/CHTLJSLexer/Lexer.h"

using namespace CHTLJS;

TEST(CHTLJSLexerTest, SingleCharacterTokens) {
    std::string source = "(){}[];:.";
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
        TokenType::DOT,
        TokenType::END_OF_FILE
    };

    for (const auto& expected_token : expected_tokens) {
        Token token = lexer.scanToken();
        EXPECT_EQ(token.type, expected_token);
    }
}

TEST(CHTLJSLexerTest, DoubleCharacterTokens) {
    std::string source = "{{ }} -> &->";
    Lexer lexer(source);

    std::vector<TokenType> expected_tokens = {
        TokenType::LEFT_DOUBLE_BRACE,
        TokenType::RIGHT_DOUBLE_BRACE,
        TokenType::ARROW,
        TokenType::AMPERSAND_ARROW_DASH,
        TokenType::END_OF_FILE
    };

    for (const auto& expected_token : expected_tokens) {
        Token token = lexer.scanToken();
        EXPECT_EQ(token.type, expected_token);
    }
}

TEST(CHTLJSLexerTest, Keywords) {
    std::string source = "Listen Delegate Vir Animate Router";
    Lexer lexer(source);

    std::vector<TokenType> expected_tokens = {
        TokenType::KEYWORD_LISTEN,
        TokenType::KEYWORD_DELEGATE,
        TokenType::KEYWORD_VIR,
        TokenType::KEYWORD_ANIMATE,
        TokenType::KEYWORD_ROUTER,
        TokenType::END_OF_FILE
    };

    for (const auto& expected_token : expected_tokens) {
        Token token = lexer.scanToken();
        EXPECT_EQ(token.type, expected_token);
    }
}

TEST(CHTLJSLexerTest, IdentifierWithHash) {
    std::string source = "#my-id";
    Lexer lexer(source);

    Token token = lexer.scanToken();
    EXPECT_EQ(token.type, TokenType::IDENTIFIER);
    EXPECT_EQ(token.lexeme, "#my-id");
}
