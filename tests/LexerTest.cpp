#include <gtest/gtest.h>
#include "../src/CHTL/CHTLLexer/Lexer.h"
#include "../src/CHTL/CHTLLexer/Token.h"

using namespace CHTL;

// Helper function to compare tokens
void assertToken(const Token& token, TokenType expectedType, const std::string& expectedLexeme, int expectedLine) {
    EXPECT_EQ(token.type, expectedType);
    EXPECT_EQ(token.lexeme, expectedLexeme);
    EXPECT_EQ(token.line, expectedLine);
}

TEST(LexerTest, SingleTokens) {
    std::string source = "{ } ( ) [ ] : ; , . = + - * / % ** > < && || -> &->";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 24);
    assertToken(tokens[0], TokenType::LEFT_BRACE, "{", 1);
    assertToken(tokens[1], TokenType::RIGHT_BRACE, "}", 1);
    assertToken(tokens[2], TokenType::LEFT_PAREN, "(", 1);
    assertToken(tokens[3], TokenType::RIGHT_PAREN, ")", 1);
    assertToken(tokens[4], TokenType::LEFT_BRACKET, "[", 1);
    assertToken(tokens[5], TokenType::RIGHT_BRACKET, "]", 1);
    assertToken(tokens[6], TokenType::COLON, ":", 1);
    assertToken(tokens[7], TokenType::SEMICOLON, ";", 1);
    assertToken(tokens[8], TokenType::COMMA, ",", 1);
    assertToken(tokens[9], TokenType::DOT, ".", 1);
    assertToken(tokens[10], TokenType::EQUAL, "=", 1);
    assertToken(tokens[11], TokenType::PLUS, "+", 1);
    assertToken(tokens[12], TokenType::MINUS, "-", 1);
    assertToken(tokens[13], TokenType::STAR, "*", 1);
    assertToken(tokens[14], TokenType::SLASH, "/", 1);
    assertToken(tokens[15], TokenType::PERCENT, "%", 1);
    assertToken(tokens[16], TokenType::STAR_STAR, "**", 1);
    assertToken(tokens[17], TokenType::GREATER, ">", 1);
    assertToken(tokens[18], TokenType::LESS, "<", 1);
    assertToken(tokens[19], TokenType::AMPERSAND_AMPERSAND, "&&", 1);
    assertToken(tokens[20], TokenType::PIPE_PIPE, "||", 1);
    assertToken(tokens[21], TokenType::ARROW, "->", 1);
    assertToken(tokens[22], TokenType::BIND_ARROW, "&->", 1);
    assertToken(tokens[23], TokenType::END_OF_FILE, "", 1);
}

TEST(LexerTest, Keywords) {
    std::string source = "text style script if else inherit delete insert from as";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 11);
    assertToken(tokens[0], TokenType::TEXT, "text", 1);
    assertToken(tokens[1], TokenType::STYLE, "style", 1);
    assertToken(tokens[2], TokenType::SCRIPT, "script", 1);
    assertToken(tokens[3], TokenType::IF, "if", 1);
    assertToken(tokens[4], TokenType::ELSE, "else", 1);
    assertToken(tokens[5], TokenType::INHERIT, "inherit", 1);
    assertToken(tokens[6], TokenType::DELETE, "delete", 1);
    assertToken(tokens[7], TokenType::INSERT, "insert", 1);
    assertToken(tokens[8], TokenType::FROM, "from", 1);
    assertToken(tokens[9], TokenType::AS, "as", 1);
    assertToken(tokens[10], TokenType::END_OF_FILE, "", 1);
}

TEST(LexerTest, AtKeywords) {
    std::string source = "@Style @Element @Var @Html @JavaScript @Chtl @Config";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 8);
    assertToken(tokens[0], TokenType::AT_STYLE, "@Style", 1);
    assertToken(tokens[1], TokenType::AT_ELEMENT, "@Element", 1);
    assertToken(tokens[2], TokenType::AT_VAR, "@Var", 1);
    assertToken(tokens[3], TokenType::AT_HTML, "@Html", 1);
    assertToken(tokens[4], TokenType::AT_JAVASCRIPT, "@JavaScript", 1);
    assertToken(tokens[5], TokenType::AT_CHTL, "@Chtl", 1);
    assertToken(tokens[6], TokenType::AT_CONFIG, "@Config", 1);
    assertToken(tokens[7], TokenType::END_OF_FILE, "", 1);
}

TEST(LexerTest, BlockKeywords) {
    std::string source = "[Custom] [Template] [Origin] [Import] [Namespace] [Configuration]";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 7);
    assertToken(tokens[0], TokenType::KEYWORD_CUSTOM, "[Custom]", 1);
    assertToken(tokens[1], TokenType::KEYWORD_TEMPLATE, "[Template]", 1);
    assertToken(tokens[2], TokenType::KEYWORD_ORIGIN, "[Origin]", 1);
    assertToken(tokens[3], TokenType::KEYWORD_IMPORT, "[Import]", 1);
    assertToken(tokens[4], TokenType::KEYWORD_NAMESPACE, "[Namespace]", 1);
    assertToken(tokens[5], TokenType::KEYWORD_CONFIGURATION, "[Configuration]", 1);
    assertToken(tokens[6], TokenType::END_OF_FILE, "", 1);
}


TEST(LexerTest, StringLiterals) {
    std::string source = "\"hello\" 'world'";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 3);
    assertToken(tokens[0], TokenType::STRING, "hello", 1);
    assertToken(tokens[1], TokenType::STRING, "world", 1);
    assertToken(tokens[2], TokenType::END_OF_FILE, "", 1);
}

TEST(LexerTest, NumberLiterals) {
    std::string source = "123 45.67 89px 1.2em";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 5);
    assertToken(tokens[0], TokenType::NUMBER, "123", 1);
    assertToken(tokens[1], TokenType::NUMBER, "45.67", 1);
    assertToken(tokens[2], TokenType::NUMBER, "89px", 1);
    assertToken(tokens[3], TokenType::NUMBER, "1.2em", 1);
    assertToken(tokens[4], TokenType::END_OF_FILE, "", 1);
}

TEST(LexerTest, Identifiers) {
    std::string source = "myVar another_var Var3";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 4);
    assertToken(tokens[0], TokenType::IDENTIFIER, "myVar", 1);
    assertToken(tokens[1], TokenType::IDENTIFIER, "another_var", 1);
    assertToken(tokens[2], TokenType::IDENTIFIER, "Var3", 1);
    assertToken(tokens[3], TokenType::END_OF_FILE, "", 1);
}

TEST(LexerTest, Comments) {
    std::string source = "// this is a comment\n# this is a gen comment\n/* block comment */";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 4);
    assertToken(tokens[0], TokenType::COMMENT, "// this is a comment", 1);
    assertToken(tokens[1], TokenType::GEN_COMMENT, "# this is a gen comment", 2);
    assertToken(tokens[2], TokenType::COMMENT, "/* block comment */", 3);
    assertToken(tokens[3], TokenType::END_OF_FILE, "", 3);
}

TEST(LexerTest, Combined) {
    std::string source = "div {\n  color: red;\n  font-size: 16px;\n}";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    ASSERT_EQ(tokens.size(), 12);
    assertToken(tokens[0], TokenType::IDENTIFIER, "div", 1);
    assertToken(tokens[1], TokenType::LEFT_BRACE, "{", 1);
    assertToken(tokens[2], TokenType::IDENTIFIER, "color", 2);
    assertToken(tokens[3], TokenType::COLON, ":", 2);
    assertToken(tokens[4], TokenType::IDENTIFIER, "red", 2);
    assertToken(tokens[5], TokenType::SEMICOLON, ";", 2);
    assertToken(tokens[6], TokenType::IDENTIFIER, "font-size", 3);
    assertToken(tokens[7], TokenType::COLON, ":", 3);
    assertToken(tokens[8], TokenType::NUMBER, "16px", 3);
    assertToken(tokens[9], TokenType::SEMICOLON, ";", 3);
    assertToken(tokens[10], TokenType::RIGHT_BRACE, "}", 4);
    assertToken(tokens[11], TokenType::END_OF_FILE, "", 4);
}
