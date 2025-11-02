#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"

TEST(LexerTest, TokenizesPunctuation) {
    CHTL::Lexer lexer("(){}[],.:;=+-*/%");
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 17);
    EXPECT_EQ(tokens[0].type, CHTL::TokenType::LeftParen);
    EXPECT_EQ(tokens[1].type, CHTL::TokenType::RightParen);
    EXPECT_EQ(tokens[2].type, CHTL::TokenType::LeftBrace);
    EXPECT_EQ(tokens[3].type, CHTL::TokenType::RightBrace);
    EXPECT_EQ(tokens[4].type, CHTL::TokenType::LeftBracket);
    EXPECT_EQ(tokens[5].type, CHTL::TokenType::RightBracket);
    EXPECT_EQ(tokens[6].type, CHTL::TokenType::Comma);
    EXPECT_EQ(tokens[7].type, CHTL::TokenType::Dot);
    EXPECT_EQ(tokens[8].type, CHTL::TokenType::Colon);
    EXPECT_EQ(tokens[9].type, CHTL::TokenType::Semicolon);
    EXPECT_EQ(tokens[10].type, CHTL::TokenType::Equals);
    EXPECT_EQ(tokens[11].type, CHTL::TokenType::Plus);
    EXPECT_EQ(tokens[12].type, CHTL::TokenType::Minus);
    EXPECT_EQ(tokens[13].type, CHTL::TokenType::Star);
    EXPECT_EQ(tokens[14].type, CHTL::TokenType::Slash);
    EXPECT_EQ(tokens[15].type, CHTL::TokenType::Percent);
    EXPECT_EQ(tokens[16].type, CHTL::TokenType::Eof);
}

TEST(LexerTest, TokenizesIdentifiers) {
    CHTL::Lexer lexer("hello world");
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, CHTL::TokenType::Identifier);
    EXPECT_EQ(tokens[0].lexeme, "hello");
    EXPECT_EQ(tokens[1].type, CHTL::TokenType::Identifier);
    EXPECT_EQ(tokens[1].lexeme, "world");
    EXPECT_EQ(tokens[2].type, CHTL::TokenType::Eof);
}

TEST(LexerTest, TokenizesNumbers) {
    CHTL::Lexer lexer("123 456.789");
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, CHTL::TokenType::Number);
    EXPECT_EQ(tokens[0].lexeme, "123");
    EXPECT_EQ(tokens[1].type, CHTL::TokenType::Number);
    EXPECT_EQ(tokens[1].lexeme, "456.789");
    EXPECT_EQ(tokens[2].type, CHTL::TokenType::Eof);
}

TEST(LexerTest, TokenizesBlockKeyword) {
    CHTL::Lexer lexer("[Template]");
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, CHTL::TokenType::BlockKeyword);
    EXPECT_EQ(tokens[0].lexeme, "[Template]");
    EXPECT_EQ(tokens[1].type, CHTL::TokenType::Eof);
}

TEST(LexerTest, TokenizesAtKeyword) {
    CHTL::Lexer lexer("@Style @Html @JavaScript");
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, CHTL::TokenType::AtStyle);
    EXPECT_EQ(tokens[0].lexeme, "@Style");
    EXPECT_EQ(tokens[1].type, CHTL::TokenType::AtHtml);
    EXPECT_EQ(tokens[1].lexeme, "@Html");
    EXPECT_EQ(tokens[2].type, CHTL::TokenType::AtJavaScript);
    EXPECT_EQ(tokens[2].lexeme, "@JavaScript");
    EXPECT_EQ(tokens[3].type, CHTL::TokenType::Eof);
}

TEST(LexerTest, TokenizesStrings) {
    CHTL::Lexer lexer("\"hello world\"");
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, CHTL::TokenType::String);
    EXPECT_EQ(tokens[0].lexeme, "\"hello world\"");
    EXPECT_EQ(tokens[1].type, CHTL::TokenType::Eof);
}

TEST(LexerTest, TokenizesLineComments) {
    CHTL::Lexer lexer("// this is a comment");
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, CHTL::TokenType::LineComment);
    EXPECT_EQ(tokens[0].lexeme, "// this is a comment");
    EXPECT_EQ(tokens[1].type, CHTL::TokenType::Eof);
}

TEST(LexerTest, TokenizesBlockComments) {
    CHTL::Lexer lexer("/* this is a block comment */");
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, CHTL::TokenType::BlockComment);
    EXPECT_EQ(tokens[0].lexeme, "/* this is a block comment */");
    EXPECT_EQ(tokens[1].type, CHTL::TokenType::Eof);
}

TEST(LexerTest, TokenizesGeneratorComments) {
    CHTL::Lexer lexer("# this is a generator comment");
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, CHTL::TokenType::GeneratorComment);
    EXPECT_EQ(tokens[0].lexeme, "# this is a generator comment");
    EXPECT_EQ(tokens[1].type, CHTL::TokenType::Eof);
}

TEST(LexerTest, HandlesUnterminatedBlockComment) {
    CHTL::Lexer lexer("/* this is an unterminated block comment");
    std::vector<CHTL::Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, CHTL::TokenType::Unexpected);
    EXPECT_EQ(tokens[1].type, CHTL::TokenType::Eof);
}
