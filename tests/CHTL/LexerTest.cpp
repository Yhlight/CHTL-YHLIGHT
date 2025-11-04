#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include <vector>
#include <iostream>

struct TokenExpectation {
    CHTL::TokenType type;
    std::string_view lexeme;
};

void runLexerTest(const std::string& source, const std::vector<TokenExpectation>& expectations) {
    CHTL::Lexer lexer(source);
    std::vector<CHTL::Token> tokens;
    CHTL::Token token;
    do {
        token = lexer.scanToken();
        tokens.push_back(token);
    } while (token.type != CHTL::TokenType::END_OF_FILE);

    ASSERT_EQ(tokens.size(), expectations.size()) << "Token count mismatch";

    for (size_t i = 0; i < expectations.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expectations[i].type) << "Token type mismatch at index " << i;
        EXPECT_EQ(tokens[i].lexeme, expectations[i].lexeme) << "Token lexeme mismatch at index " << i;
    }
}

TEST(LexerTest, SingleCharacterTokens) {
    std::string source = "{}()[]:;,.+-*/%&?@";
    std::vector<TokenExpectation> expectations = {
        {CHTL::TokenType::LEFT_BRACE, "{"},
        {CHTL::TokenType::RIGHT_BRACE, "}"},
        {CHTL::TokenType::LEFT_PAREN, "("},
        {CHTL::TokenType::RIGHT_PAREN, ")"},
        {CHTL::TokenType::LEFT_BRACKET, "["},
        {CHTL::TokenType::RIGHT_BRACKET, "]"},
        {CHTL::TokenType::COLON, ":"},
        {CHTL::TokenType::SEMICOLON, ";"},
        {CHTL::TokenType::COMMA, ","},
        {CHTL::TokenType::DOT, "."},
        {CHTL::TokenType::PLUS, "+"},
        {CHTL::TokenType::MINUS, "-"},
        {CHTL::TokenType::STAR, "*"},
        {CHTL::TokenType::SLASH, "/"},
        {CHTL::TokenType::PERCENT, "%"},
        {CHTL::TokenType::AMPERSAND, "&"},
        {CHTL::TokenType::QUESTION, "?"},
        {CHTL::TokenType::AT, "@"},
        {CHTL::TokenType::END_OF_FILE, ""},
    };
    runLexerTest(source, expectations);
}

TEST(LexerTest, MultiCharacterTokens) {
    std::string source = "** && || =";
    std::vector<TokenExpectation> expectations = {
        {CHTL::TokenType::POWER, "**"},
        {CHTL::TokenType::LOGICAL_AND, "&&"},
        {CHTL::TokenType::LOGICAL_OR, "||"},
        {CHTL::TokenType::EQUAL, "="},
        {CHTL::TokenType::END_OF_FILE, ""},
    };
    runLexerTest(source, expectations);
}

TEST(LexerTest, Comments) {
    std::string source = "// this is a comment\n"
                         "/* this is a block comment */\n"
                         "# a generator comment";
    std::vector<TokenExpectation> expectations = {
        {CHTL::TokenType::GENERATOR_COMMENT, "# a generator comment"},
        {CHTL::TokenType::END_OF_FILE, ""},
    };
    runLexerTest(source, expectations);
}

TEST(LexerTest, StringLiterals) {
    std::string source = "\"hello\" 'world'";
    std::vector<TokenExpectation> expectations = {
        {CHTL::TokenType::STRING, "\"hello\""},
        {CHTL::TokenType::STRING, "'world'"},
        {CHTL::TokenType::END_OF_FILE, ""},
    };
    runLexerTest(source, expectations);
}

TEST(LexerTest, NumberLiterals) {
    std::string source = "123 45.67";
    std::vector<TokenExpectation> expectations = {
        {CHTL::TokenType::NUMBER, "123"},
        {CHTL::TokenType::NUMBER, "45.67"},
        {CHTL::TokenType::END_OF_FILE, ""},
    };
    runLexerTest(source, expectations);
}

TEST(LexerTest, IdentifiersAndKeywords) {
    std::string source = "text style script my_var";
    std::vector<TokenExpectation> expectations = {
        {CHTL::TokenType::TEXT, "text"},
        {CHTL::TokenType::STYLE, "style"},
        {CHTL::TokenType::SCRIPT, "script"},
        {CHTL::TokenType::IDENTIFIER, "my_var"},
        {CHTL::TokenType::END_OF_FILE, ""},
    };
    runLexerTest(source, expectations);
}

TEST(LexerTest, BlockKeywords) {
    std::string source = "[Template] [Custom] [Origin]";
    std::vector<TokenExpectation> expectations = {
        {CHTL::TokenType::BLOCK_TEMPLATE, "[Template]"},
        {CHTL::TokenType::BLOCK_CUSTOM, "[Custom]"},
        {CHTL::TokenType::BLOCK_ORIGIN, "[Origin]"},
        {CHTL::TokenType::END_OF_FILE, ""},
    };
    runLexerTest(source, expectations);
}
