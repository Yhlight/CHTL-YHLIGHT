#include "gtest/gtest.h"
#include "CHTLJS/CHTLLexer/Lexer.h"

TEST(CHTLJSLexerTest, TokenizesPunctuation) {
    std::string source = "(){},.-+;*/! != = == > >= < <=";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 20);
    EXPECT_EQ(tokens[0].type, CHTLJS::TokenType::LeftParen);
    EXPECT_EQ(tokens[1].type, CHTLJS::TokenType::RightParen);
    EXPECT_EQ(tokens[2].type, CHTLJS::TokenType::LeftBrace);
    EXPECT_EQ(tokens[3].type, CHTLJS::TokenType::RightBrace);
    EXPECT_EQ(tokens[4].type, CHTLJS::TokenType::Comma);
    EXPECT_EQ(tokens[5].type, CHTLJS::TokenType::Dot);
    EXPECT_EQ(tokens[6].type, CHTLJS::TokenType::Minus);
    EXPECT_EQ(tokens[7].type, CHTLJS::TokenType::Plus);
    EXPECT_EQ(tokens[8].type, CHTLJS::TokenType::Semicolon);
    EXPECT_EQ(tokens[9].type, CHTLJS::TokenType::Star);
    EXPECT_EQ(tokens[10].type, CHTLJS::TokenType::Slash);
    EXPECT_EQ(tokens[11].type, CHTLJS::TokenType::Bang);
    EXPECT_EQ(tokens[12].type, CHTLJS::TokenType::BangEqual);
    EXPECT_EQ(tokens[13].type, CHTLJS::TokenType::Equal);
    EXPECT_EQ(tokens[14].type, CHTLJS::TokenType::EqualEqual);
    EXPECT_EQ(tokens[15].type, CHTLJS::TokenType::Greater);
    EXPECT_EQ(tokens[16].type, CHTLJS::TokenType::GreaterEqual);
    EXPECT_EQ(tokens[17].type, CHTLJS::TokenType::Less);
    EXPECT_EQ(tokens[18].type, CHTLJS::TokenType::LessEqual);
    EXPECT_EQ(tokens[19].type, CHTLJS::TokenType::Eof);
}

TEST(CHTLJSLexerTest, TokenizesCHTLLiterals) {
    std::string source = "{{ }} ->";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, CHTLJS::TokenType::LeftDoubleBrace);
    EXPECT_EQ(tokens[1].type, CHTLJS::TokenType::RightDoubleBrace);
    EXPECT_EQ(tokens[2].type, CHTLJS::TokenType::ArrowDash);
}

TEST(CHTLJSLexerTest, TokenizesIdentifiers) {
    std::string source = "x y z my_var";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].type, CHTLJS::TokenType::Identifier);
    EXPECT_EQ(tokens[0].lexeme, "x");
    EXPECT_EQ(tokens[1].type, CHTLJS::TokenType::Identifier);
    EXPECT_EQ(tokens[1].lexeme, "y");
    EXPECT_EQ(tokens[2].type, CHTLJS::TokenType::Identifier);
    EXPECT_EQ(tokens[2].lexeme, "z");
    EXPECT_EQ(tokens[3].type, CHTLJS::TokenType::Identifier);
    EXPECT_EQ(tokens[3].lexeme, "my_var");
}

TEST(CHTLJSLexerTest, TokenizesKeywords) {
    std::string source = "and class else false for fun if nil or print return super this true var while";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    ASSERT_EQ(tokens.size(), 17);
    EXPECT_EQ(tokens[0].type, CHTLJS::TokenType::And);
    EXPECT_EQ(tokens[1].type, CHTLJS::TokenType::Class);
    EXPECT_EQ(tokens[2].type, CHTLJS::TokenType::Else);
    EXPECT_EQ(tokens[3].type, CHTLJS::TokenType::False);
    EXPECT_EQ(tokens[4].type, CHTLJS::TokenType::For);
    EXPECT_EQ(tokens[5].type, CHTLJS::TokenType::Fun);
    EXPECT_EQ(tokens[6].type, CHTLJS::TokenType::If);
    EXPECT_EQ(tokens[7].type, CHTLJS::TokenType::Nil);
    EXPECT_EQ(tokens[8].type, CHTLJS::TokenType::Or);
    EXPECT_EQ(tokens[9].type, CHTLJS::TokenType::Print);
    EXPECT_EQ(tokens[10].type, CHTLJS::TokenType::Return);
    EXPECT_EQ(tokens[11].type, CHTLJS::TokenType::Super);
    EXPECT_EQ(tokens[12].type, CHTLJS::TokenType::This);
    EXPECT_EQ(tokens[13].type, CHTLJS::TokenType::True);
    EXPECT_EQ(tokens[14].type, CHTLJS::TokenType::Var);
    EXPECT_EQ(tokens[15].type, CHTLJS::TokenType::While);
}
