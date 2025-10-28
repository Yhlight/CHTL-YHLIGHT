#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include <vector>

using namespace CHTL;

void assertTokensMatch(const std::vector<Token>& tokens, const std::vector<TokenType>& expectedTypes) {
    ASSERT_EQ(tokens.size(), expectedTypes.size());
    for (size_t i = 0; i < tokens.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expectedTypes[i]);
    }
}

TEST(LexerTest, TokenizesBasicElements) {
    std::string source = R"(
        html {
            body {
                div {
                    text: "Hello, CHTL!";
                }
            }
        }
    )";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    std::vector<TokenType> expected = {
        TokenType::IDENTIFIER, TokenType::LEFT_BRACE,
        TokenType::IDENTIFIER, TokenType::LEFT_BRACE,
        TokenType::IDENTIFIER, TokenType::LEFT_BRACE,
        TokenType::TEXT, TokenType::COLON, TokenType::STRING, TokenType::SEMICOLON,
        TokenType::RIGHT_BRACE,
        TokenType::RIGHT_BRACE,
        TokenType::RIGHT_BRACE,
        TokenType::END_OF_FILE
    };
    assertTokensMatch(tokens, expected);
    EXPECT_EQ(tokens[4].lexeme, "div");
    EXPECT_EQ(tokens[8].lexeme, "Hello, CHTL!");
}

TEST(LexerTest, TokenizesAttributesAndUnquotedLiterals) {
    std::string source = "div { id: box; class: container; }";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    std::vector<TokenType> expected = {
        TokenType::IDENTIFIER, TokenType::LEFT_BRACE,
        TokenType::IDENTIFIER, TokenType::COLON, TokenType::IDENTIFIER, TokenType::SEMICOLON,
        TokenType::IDENTIFIER, TokenType::COLON, TokenType::IDENTIFIER, TokenType::SEMICOLON,
        TokenType::RIGHT_BRACE,
        TokenType::END_OF_FILE
    };
    assertTokensMatch(tokens, expected);
    EXPECT_EQ(tokens[0].lexeme, "div");
    EXPECT_EQ(tokens[2].lexeme, "id");
    EXPECT_EQ(tokens[4].lexeme, "box");
}

TEST(LexerTest, HandlesComments) {
    std::string source = R"(
        // This is a single line comment.
        div { /* this is a multi-line
               comment */ }
        # This is a generator comment
    )";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    std::vector<TokenType> expected = {
        TokenType::IDENTIFIER, TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE,
        TokenType::END_OF_FILE
    };
    assertTokensMatch(tokens, expected);
}

TEST(LexerTest, TokenizesStyleBlock) {
    std::string source = R"(
        style {
            width: 100px;
            height: 200.5px + 5%;
            color: red;
        }
    )";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    std::vector<TokenType> expected = {
        TokenType::STYLE, TokenType::LEFT_BRACE,
        // width: 100px;
        TokenType::IDENTIFIER, TokenType::COLON, TokenType::NUMBER, TokenType::IDENTIFIER, TokenType::SEMICOLON,
        // height: 200.5px + 5%;
        TokenType::IDENTIFIER, TokenType::COLON, TokenType::NUMBER, TokenType::IDENTIFIER, TokenType::PLUS, TokenType::NUMBER, TokenType::PERCENT, TokenType::SEMICOLON,
        // color: red;
        TokenType::IDENTIFIER, TokenType::COLON, TokenType::IDENTIFIER, TokenType::SEMICOLON,
        TokenType::RIGHT_BRACE,
        TokenType::END_OF_FILE
    };

    assertTokensMatch(tokens, expected);

    // Check lexemes for 'width: 100px;'
    EXPECT_EQ(tokens[2].lexeme, "width");
    EXPECT_EQ(tokens[4].lexeme, "100");
    EXPECT_EQ(tokens[5].lexeme, "px");

    // Check lexemes for 'height: 200.5px + 5%;'
    EXPECT_EQ(tokens[9].lexeme, "200.5");
    EXPECT_EQ(tokens[10].lexeme, "px");
}

TEST(LexerTest, TokenizesAllSymbols) {
    std::string source = "{}()[]:;,.=+-*/%**><&&||->?@#";
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    std::vector<TokenType> expected = {
        TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE,
        TokenType::LEFT_PAREN, TokenType::RIGHT_PAREN,
        TokenType::LEFT_BRACKET, TokenType::RIGHT_BRACKET,
        TokenType::COLON, TokenType::SEMICOLON, TokenType::COMMA, TokenType::DOT,
        TokenType::EQUAL, TokenType::PLUS, TokenType::MINUS, TokenType::STAR, TokenType::SLASH, TokenType::PERCENT,
        TokenType::STAR_STAR, TokenType::GREATER, TokenType::LESS,
        TokenType::AMPERSAND_AMPERSAND, TokenType::PIPE_PIPE, TokenType::MINUS_GREATER,
        TokenType::QUESTION, TokenType::AT, TokenType::HASH,
        TokenType::END_OF_FILE
    };
    assertTokensMatch(tokens, expected);
}
